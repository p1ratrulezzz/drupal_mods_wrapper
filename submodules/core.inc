<?php
/**
 * @file
 */

/**
 * Manages data in static variable (something like drupal_static() but much cooler)
 * @param null $name
 * @param null $value
 * @param bool $delete
 * @return null
 */
function &drupal_mods_static_storage($name = NULL, $value = NULL, $delete = FALSE) {
  static $storage = array();

  $ret = NULL;
  if ($name !== NULL && $value === NULL && isset($storage[$name])) {
    if ($delete) {
      $ret = $storage[$name];
      unset($storage[$name]);
    }
    else {
      $ret = &$storage[$name];
    }
  }
  elseif ($name !== NULL && $value !== NULL) {
    $storage[$name] = $value;
    $ret = &$storage[$name];
  }
  elseif ($name === NULL && $value !== NULL) {
    $storage = $value;
    $ret = &$storage;
  }
  elseif ($name === NULL) {
    $ret = &$storage;
  }

  return $ret;
}

function drupal_mods_list_dir($directory, &$raw = NULL) {
  $fd = opendir($directory);
  if (!$fd) {
    return FALSE;
  }

  $list = array(
    'raw' => array(),
    'list' => array(),
  );

  if ($raw) {
    $list['raw'] = &$raw;
  }

  while ($file = readdir($fd)) {
    if ($file != '.' && $file != '..') {
      $fullpath = rtrim($directory, '/\\') . '/' . $file;
      $record = pathinfo($fullpath) + array(
        'uri' => $fullpath,
        'filename' => $file,
        'parent' => $directory,
      );

      if (is_dir($fullpath)) {
        $record['children'] = drupal_mods_list_dir($fullpath, $list['raw'])->list;
      }

      $list['list'][$fullpath] =
        $list['raw'][$fullpath] = $record;
    }
  }


  return (object) $list;
}

function drupal_mods_tmpfilename($directory, $seed = '', $create_file = FALSE, $extension = NULL) {
  $tmp = ($directory . '/' . substr(md5($seed . microtime(TRUE). time() . srand(time())), 0, 8));
  $suffix = ($extension ? '.'. $extension : '');

  $i = 0;
  while (file_exists($tmp.$suffix) && $i <= 50) {
    $tmp = $tmp. ($i++) . $suffix;
  }

  $tmp .= $suffix;

  if ($create_file) {
    $fp = fopen($tmp, 'w');
    fclose($fp);
  }

  return $tmp;
}

/**
 * Manipulates with data array given by $storage parameter
 * @param $storage
 * @param null $param
 * @param null $value
 * @param bool $remove
 * @return null
 */
function &_drupal_mods_param_value_manipulation(&$storage, $param = NULL, $value = NULL, $remove = FALSE) {
  $context_rw = &$storage;
  $context = $storage;
  if ($remove) {
    if ($param !== NULL && isset($context_rw[$param])) {
      unset($context_rw[$param]);
    }
    elseif ($param == NULL) {
      $context_rw = array();
    }
  }
  else {
    $context = &$context_rw;
  }

  $retval = NULL;
  if ($param !== NULL) {
    // Getter
    $retval = &$context[$param];

    // Setter
    if ($value !== NULL) {
      $context_rw[$param] = $value;
      $context[$param] = $value;
    }
  }
  elseif ($param === NULL) {
    $retval = &$context;
  }

  return $retval;
}

function drupal_mods_core_entity_prepare_query($dsn, $fields = array(), $options = array()) {
  list ($entity_type, $bundle) = explode(':', $dsn) + array(NULL, NULL);
  $info = entity_get_info($entity_type);

  $query = db_select($info['base table'], 'base');
  $fields = field_info_instances($entity_type, $bundle);
  foreach ($fields as $_field_name => $_instance_info) {
    $_field_info = field_info_field($_field_name);
    $_tablename = _field_sql_storage_tablename($_field_info);
    $_alias = $_field_name;

    $query
      ->leftJoin($_tablename, $_alias, "{$_alias}.entity_type='{$entity_type}' AND {$_alias}.entity_id=base.{$info['entity keys']['id']}");
  }

  return $query;
}

/**
 * Allows a user with the right permissions to become the selected user.
 *
 * @param $uid
 *   The user ID to switch to.
 *
 * @return
 *   TRUE if the user was sucessfully switched, or FALSE if there was an error.
 */
function drupal_mods_core_masquerade_switch_user($uid, $mask = TRUE) {
  global $user;
  if (!is_numeric($uid)) {
    drupal_set_message(t('A user id was not correctly passed to the switching function.'));
    watchdog('masquerade', 'The user id provided to switch users was not numeric.', NULL, WATCHDOG_ERROR);
    return drupal_goto($_SERVER['HTTP_REFERER']);
  }

  $new_user = user_load($uid);
  $perm = TRUE;

  // Call logout hooks when switching from original user.
  module_invoke_all('user_logout', $user);
  drupal_session_regenerate();

  if ($mask) {
    $query = db_insert('masquerade');
    $query->fields(array(
      'uid_from' => $user->uid,
      'uid_as' => $new_user->uid,
      'sid' => session_id(),
    ));
    $query->execute();
    // switch user

    $user->masquerading = $new_user->uid;
  }

  $user = $new_user;

  // Call all login hooks when switching to masquerading user.
  $edit = array(); // Passed by reference.
  user_module_invoke('login', $edit, $user);

  return TRUE;
}

/**
 * Does system shell execution of a command.
 *   Successfully stolen from source below.
 *
 * @see http://de3.php.net/manual/en/function.system.php#39969
 * @param $command
 * @param bool $stderr
 * @return string|null
 */
function drupal_mods_core_syscall($command, $stderr = FALSE){
  $result = '';
  $suffix = $stderr ? "2>&1" : "";
  if ($proc = popen("($command){$suffix}","r")){
    while (!feof($proc)) $result .= fgets($proc, 1000);
    pclose($proc);
    return $result;
  }
}

/**
 * Sends the user to a different page.
 *
 * This issues an on-site HTTP redirect. The function makes sure the redirected
 * URL is formatted correctly.
 *
 * Usually the redirected URL is constructed from this function's input
 * parameters. However you may override that behavior by setting a
 * destination in either the $_REQUEST-array (i.e. by using
 * the query string of an URI) This is used to direct the user back to
 * the proper page after completing a form. For example, after editing
 * a post on the 'admin/content'-page or after having logged on using the
 * 'user login'-block in a sidebar. The function drupal_get_destination()
 * can be used to help set the destination URL.
 *
 * Drupal will ensure that messages set by drupal_set_message() and other
 * session data are written to the database before the user is redirected.
 *
 * This function ends the request; use it instead of a return in your menu
 * callback.
 *
 * @param $path
 *   (optional) A Drupal path or a full URL, which will be passed to url() to
 *   compute the redirect for the URL.
 * @param $options
 *   (optional) An associative array of additional URL options to pass to url().
 *   (optional) allow external = TRUE - tell if should follow external url passed in $_GET. defaults to FALSE.
 * @param $http_response_code
 *   (optional) The HTTP status code to use for the redirection, defaults to
 *   302. The valid values for 3xx redirection status codes are defined in
 *   @link http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.3 RFC 2616 @endlink
 *   and the
 *   @link http://tools.ietf.org/html/draft-reschke-http-status-308-07 draft for the new HTTP status codes: @endlink
 *   - 301: Moved Permanently (the recommended value for most redirects).
 *   - 302: Found (default in Drupal and PHP, sometimes used for spamming search
 *     engines).
 *   - 303: See Other.
 *   - 304: Not Modified.
 *   - 305: Use Proxy.
 *   - 307: Temporary Redirect.
 *
 * @see drupal_get_destination()
 * @see url()
 */
function drupal_mods_core_goto($clean = FALSE, $path = '', array $options = array(), $http_response_code = 302) {
  // A destination in $_GET always overrides the function arguments.
  // We do not allow absolute URLs to be passed via $_GET, as this can be an attack vector.
  if (!$clean && isset($_GET['destination']) && (!empty($options['allow external']) || !url_is_external($_GET['destination']))) {
    $destination = drupal_parse_url($_GET['destination']);
    $path = $destination['path'];
    $options['query'] = $destination['query'];
    $options['fragment'] = $destination['fragment'];
  }

  drupal_alter('drupal_goto', $path, $options, $http_response_code);

  // The 'Location' HTTP header must be absolute.
  $options['absolute'] = TRUE;

  $url = url($path, $options);

  header('Location: ' . $url, TRUE, $http_response_code);

  // The "Location" header sends a redirect status code to the HTTP daemon. In
  // some cases this can be wrong, so we make sure none of the code below the
  // drupal_goto() call gets executed upon redirection.
  drupal_exit($url);
}
<?php
/**
 * @file
 */

function drupal_mods_setcookie($name, $value = null, $global = FALSE, $options = array()) {
  // Merge defaults
  $options += drupal_mods_cookie_settings();
  if ($global) {
    $options['domain'] = $options['auth_cookie_domain'];
  }

  $options['httponly'] = FALSE;
  return setcookie($name, $value, time() + $options['lifetime'], $options['path'], $options['domain'], $options['secure'], $options['httponly']);
}

function drupal_mods_remove_cookie($name, $global = FALSE, $options = array()) {
  drupal_mods_setcookie($name, NULL, $global, $options);
}

/**
 * Cookie Configs should be in settings.php
 * // Cookie settings
$conf['cookie_settings'] = array(
'auth_cookie_domain' => '.eiski.loc', // Set cookie domain for global cookies. Used for auth.
//'lifetime' => 5 * 24 * 60 * 60, // optional
//'domain' => $cookie_domain, // optional
//'path' => base_path(), // optional
//'secure' => FALSE, // optional
//'httponly' => TRUE, // optional
);
 */
function drupal_mods_cookie_settings($param = NULL) {
  $defaults = array();

  // Firstly get cookie configs from settings.php
  if (!empty($GLOBALS['conf']['cookie_settings'])) {
    $defaults += $GLOBALS['conf']['cookie_settings'];
  }

  $defaults += session_get_cookie_params(); //Get configs from session
  $defaults += array(
    'auth_cookie_domain' => $defaults['domain'],
  );

  if ($param !== NULL) {
    return array_key_exists($param, $defaults) ? $defaults[$param] : NULL;
  }
  else {
    return $defaults;
  }
}


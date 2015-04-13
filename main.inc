<?php
/**
 * @file
 * The baddest module of any project. Marvelous! :-)
 */

require_once DRUPAL_MODS_VENDOR. '/autoload.php';
require_once DRUPAL_MODS_ENV_DIR. '/include/rules.inc';
require_once DRUPAL_MODS_ENV_DIR. '/include/drupal_mods.views.inc';


/**
 * Implements hook_module_implements_alter()
 */
function drupal_mods_module_implements_alter(&$implementations, $hook) {
  $sanbox = array(
    'params' => &$implementations,
  );

  drupal_mods_call_func('main_core', 'hook_implements_alter', $sanbox);
}

/**
 * Implementation of hook_ctools_plugin_directory() to let the system know
 * where all our own plugins are.
 */
function drupal_mods_ctools_plugin_directory($owner, $plugin_type) {
  if ($owner == 'ctools') {
    return 'plugins/' . $plugin_type;
  }
}

/**
 * Implements hook_permission()
 * @fixme: use drupal_mods_call_func
 */
function drupal_mods_permission() {
  $perms = array();

  return $perms;
}

/**
 * Implements hook_permission()
 */
function drupal_mods_init() {
  drupal_mods_call_func('main_core', 'hook_init');
}

/**
 * Implements hook_library()
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_library() {
  $libraries = array();

  $path = drupal_get_path('module', 'eiski_common');
  $libraries['jquery.tiptip'] = array(
    'title' => 'jQuery TipTip',
    'website' => 'http://code.drewwilson.com/entry/tiptip-jquery-plugin',
    'version' => '1.3',
    'js' => array(
      "{$path}/libraries/jquery/tiptip/jquery.tipTip.js"  => array(),
    ),
    'css' => array(
      "{$path}/libraries/jquery/tiptip/tipTip.css"  => array(),
    ),
  );

  $libraries['jquery.tooltip'] = array(
    'title' => 'jQuery Tooltip',
    //'website' => 'http://code.drewwilson.com/entry/tiptip-jquery-plugin',
    //'version' => '1.3',
    'js' => array(
      "{$path}/libraries/jquery/jquery-tooltip/jquery.tooltip.min.js"  => array(),
    ),
    'css' => array(
      "{$path}/libraries/jquery/jquery-tooltip/jquery.tooltip.css"  => array(),
    ),
  );

  $libraries['jquery.fastLiveFilter'] = array(
    'title' => 'jQuery fastLiveFilter',
    'website' => 'https://github.com/awbush/jquery-fastLiveFilter',
    'version' => '1.0.3',
    'js' => array(
      "{$path}/libraries/jquery/jquery-fastLiveFilter/jquery.fastLiveFilter.js"  => array(),
    ),
//    'css' => array(
//      "{$path}/libraries/jquery/jquery-fastLiveFilter/jquery.tooltip.css"  => array(),
//    ),
  );

  return $libraries;
}

/**
 * Implements hook_form_alter()
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_form_alter(&$form, &$form_state, $form_id) {}

/**
 * @fixme use drupal_mods_call_func()
 */
function drupal_mods_field_widget_form_alter(&$element, &$form_state, &$context) {}

/**
 * Implements hook_theme()
 */
function drupal_mods_theme() {
  return array();
}


/**
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_preprocess_form_element_label(&$variables) {}



/**
 * Implementation of hook_menu().
 */
function drupal_mods_menu() {
  return drupal_mods_call_func('main_core', 'hook_menu');
}

/**
 * Loads module from submodules directory
 * @param $module
 * @param $silent (optional)
 *  Set to TRUE if it's needed to prevent throwing an exception. Defaults to FALSE.
 * @return mixed
 * @throws Exception
 * @note this function shouldn't be replaced anywhere
 */
function drupal_mods_module($module, $silent = FALSE) {
  $cache = &drupal_mods_static(__FUNCTION__, array());
  $cache_key = $module;
  $cached_cwd = NULL;
  if (!isset($cache[$cache_key])) {
//    if ($cached_cwd === NULL) {
//      $cached_cwd = __DIR__;
//    }

    $filepath = DRUPAL_MODS_SUBMODULES_DIR. '/'. $module. '.inc';
    $cache[$cache_key] = file_exists($filepath);
    //If file not exists
    if (!$cache[$cache_key] && !$silent) {
      $backtrace = debug_backtrace(~DEBUG_BACKTRACE_PROVIDE_OBJECT);
      $trace = array();
      foreach (array_reverse($backtrace) as $tr_info) {
        $trace[] = 'function '. $tr_info['function'];
      }

      $message = "drupal_mods module named \"{$module}\" was not found in \"{$filepath}\"" . "<br />\r\nTrace:<br />\r\n"
        . implode("<br />\r\n", $trace);

      drupal_set_message($message);
      throw new Exception($message);
    }

    if ($cache[$cache_key]) {
      //Now we know that file exists, so include it
      require_once $filepath;
    }
  }

  return $cache[$cache_key];
}

/**
 * Calls func from module
 * @param $module
 * @param $callable
 * @param null $sandbox
 * @param array $default
 * @return mixed|null
 * @throws Exception
 */
function &drupal_mods_call_func($module, $callable, &$sandbox = NULL, $default = array()) {
  $func = is_array($callable) ? $callable : "drupal_mods_{$module}_{$callable}";
  if (drupal_mods_module($module, TRUE) && is_callable($func)) {
    if (is_array($sandbox) && array_key_exists('params', $sandbox) && is_array($sandbox['params'])) {
      return call_user_func_array($func, $sandbox['params']);
    }
    else {
      return call_user_func($func);
    }
  }

  return $default;
}

/**
 * @param $module
 * @return bool
 */
function drupal_mods_module_loaded($module) {
  $cache = &drupal_mods_static('drupal_mods_module', array());
  $cache_key = $module;

  return isset($cache[$cache_key]);
}

/**
 * Implements hook_page_delivery_callback_alter()
 */
function drupal_mods_page_delivery_callback_alter(&$callback) {
}

function drupal_mods_core_user_is_admin($account = NULL) {
  //$account = $account === NULL ? $_GET['user'] : $account;
  return user_access('administer', $account);
}

/**
 * Implements hook_js_alter().
 */
function drupal_mods_js_alter(&$javascript) {}

/**
 * Implements hook_views_api().
 */
function drupal_mods_views_api() {
  return array(
    'api' => 3,
    'path' => drupal_get_path('module', 'drupal_mods') . '/views',
  );
}

///**
// * Implements hook_language_negotiation_info().
// * @fixme to main_core
// */
//function drupal_mods_language_negotiation_info() {
//  $file = str_replace(DRUPAL_ROOT . '/', '', drupal_mods). '/drupal_mods.language.inc';
//  $providers = array();
//
//  $providers['od-common-'. LOCALE_LANGUAGE_NEGOTIATION_URL] = array(
//    'types' => array(LANGUAGE_TYPE_CONTENT, LANGUAGE_TYPE_INTERFACE, LANGUAGE_TYPE_URL),
//    'callbacks' => array(
//      'language' => 'drupal_mods_language_language_from_session',
//      'switcher' => 'drupal_mods_language_switcher_links', // But use url links. Sly move dude :-)
//      'url_rewrite' => 'locale_language_url_rewrite_session',
//    ),
//    'file' => $file,
//    'weight' => -8,
//    'name' => t('Session mod (also counts anonymous users)'),
//    'description' => t('Determine the language from the URL (Path prefix or domain).'),
//    'config' => 'admin/config/regional/language/configure/url',
//  );
//
//  return $providers;
//}

/**
 * @note Should be enabled if drupal_mods_language_negotiation_info() is uncommented
 */
//function drupal_mods_language_switcher_links($type, $path) {
//  $languages = language_list('enabled');
//  $links = array();
//  foreach ($languages[1] as $language) {
//    $links[$language->language] = array(
//      'href' => $_GET['q'],
//      'title' => $language->native,
//      'language' => $language,
//      'attributes' => array('class' => array('language-link')),
//      'query' => array('language' => $language->language),
//    );
//  }
//
//  return $links;
//}

function drupal_mods_preprocess(&$vars, $hook) {
  //dpm($hook);
}

// #11562 Use for gk page.
/**
 * Implements hook_field_extra_fields().
 */

function drupal_mods_field_extra_fields() {}

/**
 * Implements hook_node_view().
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_node_view($node, $view_mode, $langcode) {}

/**
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_preprocess_form_element(&$variables) {}

/**
 * @fixme: use drupal_mods_call_func()
 */
function drupal_mods_process_form_element(&$variables) {}

/*
 * echo drupal_mods_plural_form(42, array('арбуз', 'арбуза', 'арбузов'))
 * @fixme: To submodule
 */
function drupal_mods_plural_form($n, $forms) {
  return $n % 10 == 1 && $n % 100 != 11 ? $n . ' ' . $forms[0] : ($n % 10 >= 2 && $n % 10 <= 4 && ($n % 100 < 10 || $n % 100 >= 20) ? $n . ' ' . $forms[1] : $n . ' ' . $forms[2]);
}

/**
 * Smart serializer
 * @param $object
 * @return string
 *
 * @todo: Make it really smart
 */
function drupal_mods_serialize($object) {
  return json_encode($object);
}

/**
 * Alias to drupal_static()
 * @param $name
 * @param null $default_value
 * @param bool $reset
 * @return mixed
 */
function &drupal_mods_static($name, $default_value = NULL, $reset = FALSE) {
  static $data = array(), $default = array();
  // First check if dealing with a previously defined static variable.
  if (isset($data[$name]) || array_key_exists($name, $data)) {
    // Non-NULL $name and both $data[$name] and $default[$name] statics exist.
    if ($reset) {
      // Reset pre-existing static variable to its default value.
      $data[$name] = $default[$name];
    }
    return $data[$name];
  }
  // Neither $data[$name] nor $default[$name] static variables exist.
  if (isset($name)) {
    if ($reset) {
      // Reset was called before a default is set and yet a variable must be
      // returned.
      return $data;
    }
    // First call with new non-NULL $name. Initialize a new static variable.
    $default[$name] = $data[$name] = $default_value;
    return $data[$name];
  }
  // Reset all: ($name == NULL). This needs to be done one at a time so that
  // references returned by earlier invocations of drupal_static() also get
  // reset.
  foreach ($default as $name => $value) {
    $data[$name] = $value;
  }
  // As the function returns a reference, the return should always be a
  // variable.
  return $data;
}

function drupal_mods_xautoload(\Drupal\xautoload\Adapter\LocalDirectoryAdapter $adapter) {
  $adapter->addPear(
    '',
    array(
      'src_custom/pear/contrib',
      'src_custom/pear/custom'
    )
  );

  // Setup pear dirs

}
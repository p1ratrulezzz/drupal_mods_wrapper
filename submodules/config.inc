<?php
/**
 * Eiski common functionality
 */

/**
 * Gets user groups filtered by passed type (node bundle)
 * @param $uid
 * @param array $options
 *  bundle (optional) =  node_bundle_name, //To filter by bundle
 * @return array|null
 */
function drupal_mods_user_memberships($uid, $options = array()) {
  // Apply defaults
  $options += array(
    'option' => 'test',
  );

  $memberships = gcc_membership_get_user_memberships($uid);
  $groups = array();
  foreach ($memberships as $_key => $membership) {
    $wrap = entity_metadata_wrapper($membership->entity_type, $membership->entity_id);
    if (!isset($options['bundle']) || ($wrap->getBundle() == $options['bundle'])) {
      $groups[$_key] = $membership;
    }

    unset($wrap);
    $wrap = null;
  }

  return !empty($groups) ? $groups : NULL;
}

/**
 * Config lazy getter
 * @param null $param
 * @return LazyAccess|LazyAccessTyped
 * @throws Exception
 */
function drupal_mods_config($param = NULL) {
  static $la = NULL;
  if ($la === NULL) {
    global $conf;
    $storage = $conf;
    drupal_mods_module('lazyaccess.class');
    $la = new LazyAccessTyped($storage);
  }

  return $param !== NULL ? $la[$param] : $la;
}
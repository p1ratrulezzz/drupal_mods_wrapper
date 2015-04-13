<?php
/*
 * @file
 */

function ajax_command_setval($selector, $value, $settings = NULL) {
  drupal_add_js(drupal_get_path('module', 'drupal_mods'). '/js/commands/drupal_mods.ajax_command_setval.js');
  return array(
    'command' => 'setval',
    'selector' => $selector,
    'value' => $value,
    'settings' => $settings,
  );
}
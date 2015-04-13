<?php
/*
 * @file
 */

function drupal_mods_rules_action_info() {
  return array(
    'generate_password' => array(
      'label' => t('Generate password'),
      'parameter' => array(
        'length' => array(
          'type' => 'integer',
          'label' => t('Length'),
        ),
      ),
      'group' => t('Gosbook'),
      'base' => 'drupal_mods_rules_actions_generate_pass',
      'provides' => array(
        'password_generated' => array(
          'type' => 'text',
          'label' => t('Generated password'),
        ),
      ),
    ),
    'set_users_password' => array(
      'label' => t('Set users password'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'password' => array(
          'type' => 'text',
          'label' => t('Password'),
        ),
      ),
      'group' => t('User'),
      'base' => 'drupal_mods_rules_actions_set_user_password',
    ),
    'drupal_mods_format_date' => array(
      'label' => t('Format timestamp to appropriate format'),
      'parameter' => array(
        'timestamp' => array(
          'type' => 'date',
          'label' => t('Valid unix timestamp'),
        ),
        'format' => array(
          'label' => t('Select date format'),
          'type' => 'text',
          'options list' => '_drupal_mods_callbacks_date_formats',
        ),
      ),
      'provides' => array(
        'date_formated' => array(
          'type' => 'text',
          'label' => t('Formated date string'),
        ),
      ),
      'group' => t('Gosbook'),
      'base' => 'drupal_mods_rules_actions_drupal_mods_format_date',
    ),
    'drupal_mods_entity_list_get_element_by_index' => array(
      'label' => t('Gets an element from list<node> by index'),
      'parameter' => array(
        'list' => array(
          'type' => 'list<node>',
          'label' => t('A list of entities'),
        ),
        'index' => array(
          'label' => t('An index to return an element'),
          'type' => 'integer',
        ),
      ),
      'provides' => array(
        'entity' => array(
          'type' => 'list_element',
          'label' => t('List element'),
        ),
      ),
      'group' => t('Gosbook'),
      'base' => 'drupal_mods_rules_actions_drupal_mods_entity_list_get_element_by_index',
    ),
    'drupal_mods_add_gcc_role_to_user' => array(
      'label' => t('Add GCC role to user'),
      'parameter' => array(
        'group_id' => array(
          'type' => 'integer',
          'label' => t('A GCC group id'),
        ),
        'uid' => array(
          'type' => 'integer',
          'label' => t('User uid'),
        ),
        'rid' => array(
          'type' => 'text',
          'label' => t('Role id'),
        ),
      ),
      'group' => t('Gosbook'),
      'base' => 'drupal_mods_add_gcc_role_to_user_action',
    ),
  );
}

function drupal_mods_add_gcc_role_to_user_action($group_id, $uid, $rid) {
  gcc_membership_add_role('node', $group_id, $uid, $rid);
}

function drupal_mods_rules_actions_drupal_mods_entity_list_get_element_by_index($entities, $index) {
  $ret = array(
    'list_element' => NULL,
  );

  if (isset($entities[$index])) {
    $ret['list_element'] = $entities[$index];
  }

  return $ret;
}

function _drupal_mods_callbacks_date_formats() {
  module_load_include('admin.inc', 'system');
  $formats_trash = system_date_time_settings();

  $options = array();
  foreach ($formats_trash['formats']['format'] as $format_name => $element) {
    $options[$format_name] = $element['#title'] . '('. date($element['#default_value']). ')';
  }

  return $options;
}

function drupal_mods_rules_actions_drupal_mods_format_date($timestamp, $format_name) {
  return array('date_formated' => format_date($timestamp, str_replace('date_format_', '', $format_name)));
}

function drupal_mods_rules_actions_generate_pass($length = 10) {
  $info = array();
  $info['password_generated'] = user_password($length);

  return $info;
}

function drupal_mods_rules_actions_set_user_password($account, $password) {
  $edit = array(
    'pass' => $password,
  );
  user_save($account, $edit);
}


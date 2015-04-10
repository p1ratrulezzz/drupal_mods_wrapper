<?php

function gb_common_disable_form_field(&$form, $field_name) {
  if(isset($form[$field_name])) {
    $form[$field_name]['#access'] = FALSE;
  }
}


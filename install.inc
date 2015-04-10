<?php

function gb_common_schema_alter(&$schema) {
  $schema['taxonomy_term_data']['fields']['name']['length'] = 2047;
}

/**
 * Add the ui_limit table to the fields table.
 */
function gb_common_update_7101() {
  db_query("ALTER TABLE `taxonomy_term_data` CHANGE `name` `name` VARCHAR(2047)");
}

<?php

/*
 * You must implement hook_migrate_api(), setting the API level to 2, for
 * your migration classes to be recognized by the Migrate module.
 */
function migrate_landing_page_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'content_migration' => array(
        'title' => t('Landing Page'),
      ),
    ),
    'migrations' => array(),
  );


  $common_arguments = array(
    'source_connection' => 'migrate',
    'source_version' => 7,
    'group_name' => 'content_migration',
  );

  $node_arguments = array(
    'Full_Width_Image' => array(
      'class_name' => 'FullWidthImage',
      'description' => t('Migration of Full Width Image Paragraph Entities'),
      'source_type' => 'full_width_image',
      'destination_type' => 'full_width_image',
    ),
    'Full_Width_Text' => array(
      'class_name' => 'FullWidthText',
      'description' => t('Migration of Full Width Text Paragraph Entities'),
      'source_type' => 'full_width_text',
      'destination_type' => 'full_width_text',
    ),
    'Pull_Quote' => array(
      'class_name' => 'PullQuote',
      'description' => t('Migration of Full Width Image Paragraph Entities'),
      'source_type' => 'pull_quote',
      'destination_type' => 'pull_quote',
    ),
    'page' => array(
      'class_name' => 'BasicPageNodeMigration',
      'description' => t('Migration of Basic Page with Paragraph Bundles'),
      'source_type' => 'page',
      'destination_type' => 'page',
      'dependencies' => array('Full_Width_Image', 'Full_Width_Text', 'Pull_Quote'),
    ),
  );

  foreach ($node_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  return $api;
}

<?php

/**
 * Saves a file object to the database.
 *
 * If the $file->fid is not set a new record will be added.
 *
 * @param $file
 *   A file object returned by file_load().
 *
 * @return
 *   The updated file object.
 *
 * @see hook_file_insert()
 * @see hook_file_update()
 */
function custom_file_save(stdClass $file) {
  $file->timestamp = REQUEST_TIME;

  $uri = FileMigration::SOURCE_DIR . str_replace('public://', '', $file->uri);
  $file->filesize = file_exists($uri) ? filesize($uri) : 1;

  // Load the stored entity, if any.
  if (!empty($file->fid) && !isset($file->original)) {
    $file->original = entity_load_unchanged('file', $file->fid);
  }

  module_invoke_all('file_presave', $file);
  module_invoke_all('entity_presave', $file, 'file');

  if (empty($file->fid)) {
    drupal_write_record('file_managed', $file);
    // Inform modules about the newly added file.
    module_invoke_all('file_insert', $file);
    module_invoke_all('entity_insert', $file, 'file');
  }
  else {
    drupal_write_record('file_managed', $file, 'fid');
    // Inform modules that the file has been updated.
    module_invoke_all('file_update', $file);
    module_invoke_all('entity_update', $file, 'file');
  }

  // Clear internal properties.
  unset($file->original);
  // Clear the static loading cache.
  entity_get_controller('file')->resetCache(array($file->fid));

  return $file;
}


/**
 * The simplest possible file class - where the value is a remote URI which
 * simply needs to be saved as the URI on the destination side, with no attempt
 * to copy or otherwise use it.
 */
class CustomMigrateFileUriAsIs extends MigrateFileBase {
  public function processFile($value, $owner) {
    $file = custom_file_save($this->createFileEntity($value, $owner));
    return $file;
  }
}


class FileMigration extends DrupalFile7Migration {

  const SOURCE_DIR = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';

  /**
   * FileMigration constructor.
   * @param array $arguments
   */
  public function __construct(array $arguments) {

    $arguments['source_dir'] = self::SOURCE_DIR;
    $arguments['file_class'] = 'CustomMigrateFileUriAsIs';

    parent::__construct($arguments);

  }

  /**
   * Called after the query data is fetched - we'll use this to populate the
   * source row with the CCK fields.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $this->destination->setFileClass('CustomMigrateFileUriAsIs');
  }

  /**
   * Remove the standard stream wrappers from the incoming URI.
   *
   * @param $uri
   *
   * @return string
   */
  protected function fixUri($uri) {
    return $uri;
  }
}
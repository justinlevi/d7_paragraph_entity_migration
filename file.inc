<?php

class FileMigration extends DrupalMigration {

  /**
   * FileMigration constructor.
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $arguments['source_dir'] = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';
    $arguments['file_class'] = 'MigrateFileUriAsIs';

    $this->addFieldMapping('value', 'uri')
      ->callbacks(array($this, 'fixUri'));
    $this->addFieldMapping('destination_file', 'uri')
      ->callbacks(array($this, 'fixUri'));
    $this->addFieldMapping('timestamp', 'timestamp');

    $this->addUnmigratedSources(array('status'));

    //$this->destination = new MigrateFileUriAsIs($arguments);




    $arguments['bundle'] = 'file';
    $arguments['destination_dir'] = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';

    $this->sourceFields += $this->version->getSourceFields('file',
      $arguments['bundle']);
    // Allow derived classes to override this definition by setting it before
    // calling their parent constructor
    if (!isset($this->map)) {
      $this->map = new MigrateSQLMap($this->machineName,
        array(
          'fid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Source file ID',
            'alias' => 'f',
          ),
        ),
        MigrateDestinationFile::getKeySchema(),
        $this->mapConnection
      );
    }
    $this->source = new MigrateSourceSQL($this->query(),
      $this->sourceFields, NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationFile($arguments['bundle'], $arguments['file_class']);

    // Setup common mappings
    $this->addFieldMapping('destination_dir')
      ->defaultValue($arguments['destination_dir']);
    $this->addFieldMapping('source_dir')
      ->defaultValue($arguments['source_dir']);
    $this->addFieldMapping('file_replace')
      ->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
    $this->addFieldmapping('preserve_files')
      ->defaultValue(FALSE);

    if (isset($arguments['default_uid'])) {
      $default_uid = $arguments['default_uid'];
    }
    else {
      $default_uid = 1;
    }
    if (isset($user_migration)) {
      $this->addFieldMapping('uid', 'uid')
        ->sourceMigration($user_migration)
        ->defaultValue($default_uid);
    }
    else {
      $this->addFieldMapping('uid')
        ->defaultValue($default_uid);
    }
    $this->addUnmigratedSources(array('filename', 'filemime', 'filesize', 'urlencode'));
  }

  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('file_managed', 'f')
      ->fields('f')
      ->orderBy($this->newOnly ? 'f.fid' : 'f.timestamp');
    return $query;
  }

  /**
   * Called after the query data is fetched - we'll use this to populate the
   * source row with the CCK fields.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $this->version->getSourceValues($row, $row->fid);
    $this->destination->setFileClass('MigrateFileUriAsIs');
  }

  protected function fixUri($uri) {

    $dir = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';
    $result = str_replace('public://', '', $uri);
    $result = str_replace('private://', '', $result);
    return $dir . $result;
  }

}
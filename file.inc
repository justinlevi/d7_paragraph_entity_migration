<?php

class FileMigration extends DrupalFile7Migration {
  public function __construct(array $arguments) {
    //$arguments['source_dir'] = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';

    $arguments += array(
      'description' => 'File migration',
      'destination_dir' => 'public://',
      'source_dir' => 'public://',
      //'source_dir' => '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/',
      'file_class' => 'MigrateFileUriAsIs',
    );

    parent::__construct($arguments);
  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $this->version->getSourceValues($row, $row->fid);

    $this->destination->setFileClass('MigrateFileUri');
    $row->destination_dir = "public://";
  }

}
<?php

class FileMigration extends DrupalFile7Migration {
  public function __construct(array $arguments) {
    $arguments['source_dir'] = '/Users/justinwinter/Sites/d7a/docroot/sites/default/files/';

    parent::__construct($arguments);
  }
}
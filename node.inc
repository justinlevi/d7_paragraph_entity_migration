<?php

class BasicPageNodeMigration extends DrupalNode7Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->sourceFields['field_slice'] = 'Slice';
    $this->sourceFields['field_slice:revision_id'] = 'Slice';

    $this->addFieldMapping('field_slice', 'field_slice')
         ->sourceMigration(array('Full_Width_Image', 'Full_Width_Text', 'Pull_Quote'));
    $this->addFieldMapping('field_slice:revision_id', 'field_slice:revision_id');
  }
}

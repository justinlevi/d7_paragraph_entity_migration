<?php

/**
 * Base functionality for all paragraph bundle types.
 */
class ParagraphItem extends DrupalMigration {

  /**
   * Options for constructing the destination.
   */
  protected $paragraph_options = array();

  /**
   * The source node type.
   *
   * @var string
   */
  protected $sourceType;

  /**
   * The destination entity type.
   *
   * @var string
   */
  protected $entityType;

  /**
   * The destination bundle.
   *
   * @var string
   */
  protected $bundle;


  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {

    // Need to set these prior to initializing the parent class
    $this->entityType = 'paragraphs_item';
    $this->bundle = $arguments['destination_type'];

    parent::__construct($arguments);

    // Source Fields
    $this->sourceFields += $this->version->getSourceFields('paragraphs_item', $this->bundle);

    $this->source = new MigrateSourceSQL($this->query($this->bundle), $this->sourceFields, NULL,
      $this->sourceOptions);

    // Create a migration map.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'item_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationParagraphsItem::getKeySchema()
    );

    // The field we're working with
    $paragraph_options['field_name'] = 'field_slice';

    // Set a destination.
    $this->destination = new MigrateDestinationParagraphsItem(
      $this->bundle,
      $this->paragraph_options
    );

    $this->sourceFields['bundle'] = 'bundle';

    // The field mappings
    $this->addFieldMapping('field_name', 'field_name');
    //$this->addFieldMapping('bundle', 'bundle');

    foreach ($this->sourceFields as $machine_name => $label){
      if($machine_name !== 'field_image' &&
         $machine_name !== 'bundle' &&
         strpos($machine_name, 'language') == false ){

        $this->addFieldMapping($machine_name, $machine_name);
      }
    }
  }

  /**
   * Called after the query data is fetched - we'll use this to populate the
   * source row with the CCK fields.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $this->version->getSourceValues($row, $row->revision_id);
  }


  /**
   * Query for paragraph bundles from Drupal 7.
   *
   * @return QueryConditionInterface
   */
  protected function query($bundle="") {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('paragraphs_item', 'p')
      ->fields('p', array('item_id', 'revision_id', 'bundle', 'field_name'))
      ->condition('bundle', $bundle)
      ->orderBy($this->newOnly ? 'p.item_id' : 'p.revision_id');
    return $query;
  }
}


/**
 * ParagraphsBundle - full_width_image.
 */
class FullWidthImage extends ParagraphItem {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Full Width Image Paragraph Item.');

    // source : https://www.drupal.org/node/1540106#comment-8589917
    $this->addFieldMapping('field_image', 'field_image')->sourceMigration('Files');
    $this->addFieldMapping('field_image:file_class')->defaultValue('migrateFileFid');
    $this->addFieldMapping('field_image:preserve_files')->defaultValue(FALSE);
  }
}

/**
 * ParagraphsBundle - full_width_image.
 */
//class FullWidthText extends ParagraphItem {
//
//  /**
//   * {@inheritdoc}
//   */
//  public function __construct($arguments) {
//    parent::__construct($arguments);
//
//    $this->description = t('Full Width Text Paragraph Item.');
//  }
//}

/**
 * ParagraphsBundle - pull_quote.
 */
//class PullQuote extends ParagraphItem {
//
//  /**
//   * {@inheritdoc}
//   */
//  public function __construct($arguments) {
//    parent::__construct($arguments);
//
//    $this->description = t('Pull Quote Paragraph Item.');
//  }
//}

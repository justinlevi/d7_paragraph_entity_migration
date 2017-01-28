<?php

/**
 * Base functionality for all paragraph bundle types.
 */
class ParagraphItemAbstractClass extends DrupalMigration {

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
    parent::__construct($arguments);

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

  protected function config($bundle) {
    $this->sourceFields += $this->version->getSourceFields('paragraphs_item', $bundle);
    $this->source = new MigrateSourceSQL($this->query($bundle), $this->sourceFields, NULL,
      $this->sourceOptions);

    $paragraph_options['field_name'] = 'field_slice';

    // Set a destination.
    $this->destination = new MigrateDestinationParagraphsItem(
      $bundle,
      $this->paragraph_options
    );

    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('bundle', 'bundle');

    foreach ($this->sourceFields as $machine_name => $label){
      $this->addFieldMapping($machine_name, $machine_name);
    }
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
class FullWidthImage extends ParagraphItemAbstractClass {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->entityType = 'paragraphs_item';
    $this->bundle = $arguments['destination_type'];
    parent::__construct($arguments);

    $this->description = t('Full Width Image Paragraph Item.');
    $this->config($arguments['destination_type']);

    $this->addFieldMapping('field_image:source_dir')->defaultValue('/sites/default/files/');
    $this->addFieldMapping('field_main_image:source_path')
      ->defaultValue('/mnt/source_images');
  }
}

/**
 * ParagraphsBundle - full_width_image.
 */
class FullWidthText extends ParagraphItemAbstractClass {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->entityType = 'paragraphs_item';
    $this->bundle = $arguments['destination_type'];
    parent::__construct($arguments);

    $this->description = t('Full Width Text Paragraph Item.');
    $this->config($arguments['destination_type']);
  }
}

/**
 * ParagraphsBundle - pull_quote.
 */
class PullQuote extends ParagraphItemAbstractClass {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->entityType = 'paragraphs_item';
    $this->bundle = $arguments['destination_type'];
    parent::__construct($arguments);

    $this->description = t('Pull Quote Paragraph Item.');
    $this->config($arguments['destination_type']);
  }
}

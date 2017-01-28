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
    //$this->config($arguments['destination_type']);
    $this->sourceFields += $this->version->getSourceFields('paragraphs_item', $arguments['destination_type']);
    $this->source = new MigrateSourceSQL($this->query($arguments['destination_type']), $this->sourceFields, NULL,
      $this->sourceOptions);

    $paragraph_options['field_name'] = 'field_slice';

    // Set a destination.
    $this->destination = new MigrateDestinationParagraphsItem(
      'full_width_image',
      $this->paragraph_options
    );

    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('field_image', 'field_image');
    $this->addFieldMapping('field_caption', 'field_caption');
    $this->addFieldMapping('bundle', 'bundle');
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
    //$this->config($arguments['destination_type']);
    $this->sourceFields += $this->version->getSourceFields('paragraphs_item', $arguments['destination_type']);
    $this->source = new MigrateSourceSQL($this->query($arguments['destination_type']), $this->sourceFields, NULL,
      $this->sourceOptions);

    $paragraph_options['field_name'] = 'field_slice';

    // Set a destination.
    $this->destination = new MigrateDestinationParagraphsItem(
      'full_width_text',
      $this->paragraph_options
    );

    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('field_text', 'field_text');
    $this->addFieldMapping('field_text:value', 'field_text:value');
    $this->addFieldMapping('bundle', 'bundle');
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
    //$this->config($arguments['destination_type']);
    $this->sourceFields += $this->version->getSourceFields('paragraphs_item', $arguments['destination_type']);
    $this->source = new MigrateSourceSQL($this->query($arguments['destination_type']), $this->sourceFields, NULL,
      $this->sourceOptions);

    $paragraph_options['field_name'] = 'field_slice';

    // Set a destination.
    $this->destination = new MigrateDestinationParagraphsItem(
      'pull_quote',
      $this->paragraph_options
    );

    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('field_quote', 'field_quote');
    $this->addFieldMapping('field_quote:value', 'field_quote:value');
    $this->addFieldMapping('field_credit', 'field_credit');
    $this->addFieldMapping('field_title_organization', 'field_title_organization');
    $this->addFieldMapping('bundle', 'bundle');
  }
}

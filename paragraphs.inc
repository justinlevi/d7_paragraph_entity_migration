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
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Create a migration map.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'item_id' => array(
          'type' => 'int',
          'unsigned' => FALSE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationParagraphsItem::getKeySchema()
    );
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
    parent::__construct($arguments);

    $this->description = t('Full Width Image Paragraph Item.');
    $this->config($arguments['destination_type']);

    $this->addFieldMapping('field_image', 'field_image');
    $this->addFieldMapping('field_caption', 'field_caption');
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
    parent::__construct($arguments);

    $this->description = t('Full Width Text Paragraph Item.');
    $this->config($arguments['destination_type']);

    $this->addFieldMapping('field_text', 'field_text');
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
    parent::__construct($arguments);

    $this->description = t('Pull Quote Paragraph Item.');
    $this->config($arguments['destination_type']);

    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('field_quote', 'field_quote');
    $this->addFieldMapping('field_credit', 'field_credit');
    $this->addFieldMapping('field_title_organization', 'field_title_organization');
  }
}

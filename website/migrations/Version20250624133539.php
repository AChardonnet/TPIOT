<?php

declare(strict_types=1);

namespace DoctrineMigrations;

use Doctrine\DBAL\Schema\Schema;
use Doctrine\Migrations\AbstractMigration;

/**
 * Auto-generated Migration: Please modify to your needs!
 */
final class Version20250624133539 extends AbstractMigration
{
    public function getDescription(): string
    {
        return '';
    }

    public function up(Schema $schema): void
    {
        // this up() migration is auto-generated, please modify it to your needs
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE tds_id_seq INCREMENT BY 1 MINVALUE 1 START 1
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE temperature_soil_id_seq INCREMENT BY 1 MINVALUE 1 START 1
        SQL);
        $this->addSql(<<<'SQL'
            CREATE TABLE tds (id INT NOT NULL DEFAULT nextval('tds_id_seq'), tds DOUBLE PRECISION NOT NULL, timestamp BIGINT NOT NULL, PRIMARY KEY(id))
        SQL);
        $this->addSql(<<<'SQL'
            CREATE TABLE temperature_soil (id INT NOT NULL DEFAULT nextval('temperature_soil_id_seq'), temperature_soil DOUBLE PRECISION NOT NULL, timestamp BIGINT NOT NULL, PRIMARY KEY(id))
        SQL);
    }

    public function down(Schema $schema): void
    {
        // this down() migration is auto-generated, please modify it to your needs
        $this->addSql(<<<'SQL'
            CREATE SCHEMA public
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA timescaledb_information
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA timescaledb_experimental
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_internal
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_functions
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_debug
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_config
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_catalog
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SCHEMA _timescaledb_cache
        SQL);
        $this->addSql(<<<'SQL'
            DROP SEQUENCE tds_id_seq CASCADE
        SQL);
        $this->addSql(<<<'SQL'
            DROP SEQUENCE temperature_soil_id_seq CASCADE
        SQL);
        $this->addSql(<<<'SQL'
            DROP TABLE tds
        SQL);
        $this->addSql(<<<'SQL'
            DROP TABLE temperature_soil
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE temperature_id_seq
        SQL);
        $this->addSql(<<<'SQL'
            SELECT setval('temperature_id_seq', (SELECT MAX(id) FROM temperature))
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE temperature ALTER id SET DEFAULT nextval('temperature_id_seq')
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE humidity_id_seq
        SQL);
        $this->addSql(<<<'SQL'
            SELECT setval('humidity_id_seq', (SELECT MAX(id) FROM humidity))
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE humidity ALTER id SET DEFAULT nextval('humidity_id_seq')
        SQL);
    }
}

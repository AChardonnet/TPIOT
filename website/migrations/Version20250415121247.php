<?php

declare(strict_types=1);

namespace DoctrineMigrations;

use Doctrine\DBAL\Schema\Schema;
use Doctrine\Migrations\AbstractMigration;

/**
 * Auto-generated Migration: Please modify to your needs!
 */
final class Version20250415121247 extends AbstractMigration
{
    public function getDescription(): string
    {
        return '';
    }

    public function up(Schema $schema): void
    {
        // this up() migration is auto-generated, please modify it to your needs
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE humidity_id_seq INCREMENT BY 1 MINVALUE 1 START 1
        SQL);
        $this->addSql(<<<'SQL'
            CREATE SEQUENCE temperature_id_seq INCREMENT BY 1 MINVALUE 1 START 1
        SQL);
        $this->addSql(<<<'SQL'
            CREATE TABLE messenger_messages (id BIGSERIAL NOT NULL, body TEXT NOT NULL, headers TEXT NOT NULL, queue_name VARCHAR(190) NOT NULL, created_at TIMESTAMP(0) WITHOUT TIME ZONE NOT NULL, available_at TIMESTAMP(0) WITHOUT TIME ZONE NOT NULL, delivered_at TIMESTAMP(0) WITHOUT TIME ZONE DEFAULT NULL, PRIMARY KEY(id))
        SQL);
        $this->addSql(<<<'SQL'
            CREATE INDEX IDX_75EA56E0FB7336F0 ON messenger_messages (queue_name)
        SQL);
        $this->addSql(<<<'SQL'
            CREATE INDEX IDX_75EA56E0E3BD61CE ON messenger_messages (available_at)
        SQL);
        $this->addSql(<<<'SQL'
            CREATE INDEX IDX_75EA56E016BA31DB ON messenger_messages (delivered_at)
        SQL);
        $this->addSql(<<<'SQL'
            COMMENT ON COLUMN messenger_messages.created_at IS '(DC2Type:datetime_immutable)'
        SQL);
        $this->addSql(<<<'SQL'
            COMMENT ON COLUMN messenger_messages.available_at IS '(DC2Type:datetime_immutable)'
        SQL);
        $this->addSql(<<<'SQL'
            COMMENT ON COLUMN messenger_messages.delivered_at IS '(DC2Type:datetime_immutable)'
        SQL);
        $this->addSql(<<<'SQL'
            CREATE OR REPLACE FUNCTION notify_messenger_messages() RETURNS TRIGGER AS $$
                BEGIN
                    PERFORM pg_notify('messenger_messages', NEW.queue_name::text);
                    RETURN NEW;
                END;
            $$ LANGUAGE plpgsql;
        SQL);
        $this->addSql(<<<'SQL'
            DROP TRIGGER IF EXISTS notify_trigger ON messenger_messages;
        SQL);
        $this->addSql(<<<'SQL'
            CREATE TRIGGER notify_trigger AFTER INSERT OR UPDATE ON messenger_messages FOR EACH ROW EXECUTE PROCEDURE notify_messenger_messages();
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE humidity ADD id SERIAL PRIMARY KEY
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE humidity ALTER COLUMN id SET DEFAULT nextval('humidity_id_seq')
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE temperature ADD id SERIAL PRIMARY KEY
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE temperature ALTER COLUMN id SET DEFAULT nextval('temperature_id_seq')
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
            DROP SEQUENCE humidity_id_seq CASCADE
        SQL);
        $this->addSql(<<<'SQL'
            DROP SEQUENCE temperature_id_seq CASCADE
        SQL);
        $this->addSql(<<<'SQL'
            DROP TABLE messenger_messages
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE temperature DROP CONSTRAINT temperature_pkey
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE temperature DROP id
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE humidity DROP CONSTRAINT humidity_pkey
        SQL);
        $this->addSql(<<<'SQL'
            ALTER TABLE humidity DROP id
        SQL);
    }
}

<?php

namespace App\Entity;

use App\Repository\TemperatureSoilRepository;
use Doctrine\DBAL\Types\Types;
use Doctrine\ORM\Mapping as ORM;

#[ORM\Entity(repositoryClass: TemperatureSoilRepository::class)]
class TemperatureSoil
{
    #[ORM\Id]
    #[ORM\GeneratedValue]
    #[ORM\Column]
    private ?int $id = null;

    #[ORM\Column]
    private ?float $temperatureSoil = null;

    #[ORM\Column(type: Types::BIGINT)]
    private ?string $timestamp = null;

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getTemperatureSoil(): ?float
    {
        return $this->temperatureSoil;
    }

    public function setTemperatureSoil(float $temperatureSoil): static
    {
        $this->temperatureSoil = $temperatureSoil;

        return $this;
    }

    public function getTimestamp(): ?string
    {
        return $this->timestamp;
    }

    public function setTimestamp(string $timestamp): static
    {
        $this->timestamp = $timestamp;

        return $this;
    }
}

<?php

namespace App\Entity;

use App\Repository\TdsRepository;
use Doctrine\DBAL\Types\Types;
use Doctrine\ORM\Mapping as ORM;

#[ORM\Entity(repositoryClass: TdsRepository::class)]
class Tds
{
    #[ORM\Id]
    #[ORM\GeneratedValue]
    #[ORM\Column]
    private ?int $id = null;

    #[ORM\Column]
    private ?float $tds = null;

    #[ORM\Column(type: Types::BIGINT)]
    private ?string $timestamp = null;

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getTds(): ?float
    {
        return $this->tds;
    }

    public function setTds(float $tds): static
    {
        $this->tds = $tds;

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

<?php

namespace App\Twig\Components;

use App\Entity\Humidity;
use App\Entity\Temperature;
use Doctrine\ORM\EntityManagerInterface;
use Symfony\UX\Chartjs\Builder\ChartBuilderInterface;
use Symfony\UX\Chartjs\Model\Chart;
use Symfony\UX\LiveComponent\Attribute\AsLiveComponent;
use Symfony\UX\LiveComponent\Attribute\LiveProp;
use Symfony\UX\LiveComponent\DefaultActionTrait;

#[AsLiveComponent]
class RealTimeChartDHT
{
    use DefaultActionTrait;

    #[LiveProp(writable: true)]
    public int $number = 100;

    public function __construct(
        private ChartBuilderInterface $chartBuilder,
        private EntityManagerInterface $entityManager,
    ) {}

    public function getChart(): Chart
    {
        $chart = $this->chartBuilder->createChart(Chart::TYPE_LINE);

        $dataPointsTemp = $this->entityManager->getRepository(Temperature::class)->findBy([], ['timestamp' => 'DESC'], $this->number);
        $dataPointsHumi = $this->entityManager->getRepository(Humidity::class)->findBy([], ['timestamp' => 'DESC'], $this->number);

        $labels = array_map(fn($d) => (new \DateTime())->setTimestamp($d->getTimestamp() / 1000)->format('Y-m-d H:i:s'), $dataPointsTemp);
        $valuesTemp = array_map(fn($d) => $d->getTemperature(), $dataPointsTemp);
        $valuesHumi = array_map(fn($d) => $d->getHumidity(), $dataPointsHumi);

        $chart->setData([
            'labels' => $labels,
            'datasets' => [[
                'label' => 'Temperature',
                'backgroundColor' => 'rgba(255, 0, 0, 0.44)',
                'borderColor' => 'rgb(255, 0, 0)',
                'data' => $valuesTemp,'pointRadius' => 0,
                'pointHoverRadius' => 0,
            ],[
                'label' => 'Humidity',
                'backgroundColor' => 'rgba(0, 0, 255, 0.44)',
                'borderColor' => 'rgb(0, 0, 255)',
                'data' => $valuesHumi,'pointRadius' => 0,
                'pointHoverRadius' => 0,
            ],
        ]]);
        $chart->setOptions([
            'animation' => false,
        ]);

        return $chart;
    }

    public function getMaxPoints(): int
    {
        return $this->entityManager->getRepository(Temperature::class)->count();
    }
}

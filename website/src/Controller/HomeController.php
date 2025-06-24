<?php

namespace App\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Attribute\Route;
use Doctrine\DBAL\Connection;
use Symfony\UX\Chartjs\Builder\ChartBuilderInterface;
use Symfony\UX\Chartjs\Model\Chart;
use Doctrine\ORM\EntityManagerInterface;
use App\Entity\Temperature;
use App\Entity\Humidity;
use App\Form\UserSearchType;
use App\Model\UserSearch;
use Symfony\Component\Security\Http\Attribute\IsGranted;

final class HomeController extends AbstractController
{
    #[Route('/', name: 'app_home')]
    public function index(): Response
    {
        return $this->render('home/index.html.twig', []);
    }

    #[Route('/dht', name: 'dht_graph')]
    public function dhtGraph(): Response
    {
        return $this->render('home/dht.html.twig', []);
    }
    #[Route('/tds', name: 'tds_graph')]
    public function tdsGraph(): Response
    {
        return $this->render('home/tds.html.twig', []);
    }

    #[Route('/users', name: 'search_users')]
    public function searchUsers(Request $request, Connection $connection): Response
    {
        $userSearch = new UserSearch();
        $form = $this->createForm(UserSearchType::class, $userSearch);
        $users = [];

        $form->handleRequest($request);
        if ($form->isSubmitted() && $form->isValid()) {
            $sql = "SELECT username, email FROM app_user WHERE app_user.email = '".$userSearch->getEmail()."'";
            $results = $connection->fetchAllAssociative($sql);
            $users = $results;
        }

        return $this->render('home/users.html.twig', [
            'form' => $form->createView(),
            'users' => $users,
        ]);
    }

    #[Route('/admin', name: 'admin')]
    #[IsGranted('ROLE_ADMIN')]
    public function admin(): Response
    {
        return $this->render('home/admin.html.twig', []);
    }
}

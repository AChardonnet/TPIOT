<?php

namespace App\Controller;

use App\Entity\User;
use App\Form\RegistrationType;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\Persistence\ManagerRegistry;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Attribute\Route;

final class SecurityController extends AbstractController
{
    #[Route("/signup", name: "security_registration")]
    public function registration(Request $request, EntityManagerInterface $entityManager): Response
    {
        $user = new User();
        $form = $this->createForm(RegistrationType::class, $user);

        $form->handleRequest($request);
        if ($form->isSubmitted() && $form->isValid()) {
            $user->setRoles(['ROLE_USER']);
            
            if ($form->isSubmitted()) {
                dump($form->getErrors(true));
            }

            $entityManager->persist($user);
            $entityManager->flush();

            return $this->redirectToRoute('security_login');
        }

        return $this->render('security/registration.html.twig', [
            'form' => $form,
        ]); 
    }

    #[Route("/login", name: "security_login")]
    public function login(): Response
    {
        return $this->render('security/login.html.twig');
    } 

    #[Route("/logout", name: "security_logout")]
    public function logout(): void
    {
        // Symfony will intercept this route and handle the logout process
    }
}


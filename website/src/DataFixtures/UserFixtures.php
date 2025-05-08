<?php

namespace App\DataFixtures;

use App\Entity\User;
use Doctrine\Bundle\FixturesBundle\Fixture;
use Doctrine\Persistence\ObjectManager;
use Faker\Factory;

class UserFixtures extends Fixture
{
    public function __construct()
    {
    }

    public function load(ObjectManager $manager): void
    {
        $faker = Factory::create();

        for ($i = 0; $i < 100; $i++) {
            $user = new User();
            $user->setEmail($faker->unique()->safeEmail());
            $user->setPassword($faker->password());
            $user->setUsername($faker->userName());
            $user->setRoles(['ROLE_USER']);

            $manager->persist($user);
        }
        $user = new User();
        $user->setEmail('JeanLoutre007@ginfo.centrale-med.fr');
        $user->setPassword('ILoveSalmonAndIHateVibly');
        $user->setUsername('admin');
        $user->setRoles(['ROLE_ADMIN','ROLE_USER']);
        $manager->persist($user);

        $manager->flush();
    }
}

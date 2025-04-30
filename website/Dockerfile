FROM php:8.2-apache

RUN apt-get update && apt-get install -y \
    git zip unzip libpng-dev libzip-dev libpq-dev nodejs npm

RUN docker-php-ext-install pdo pdo_pgsql pgsql zip gd

RUN a2enmod rewrite

WORKDIR /var/www

COPY . /var/www/

RUN chown -R www-data:www-data /var/www/var

COPY --from=composer:latest /usr/bin/composer /usr/bin/composer

RUN COMPOSER_ALLOW_SUPERUSER=1 composer install --no-scripts --no-autoloader

RUN echo "memory_limit = -1" > /usr/local/etc/php/conf.d/memory-limit.ini

RUN npm i 

RUN npm run build

EXPOSE 80 

RUN sed -i 's!/var/www/html!/var/www/public!g' \
    /etc/apache2/sites-available/000-default.conf

CMD ["apache2-foreground"]
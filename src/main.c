/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/21 12:49:24 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"


int main(int ac, char **av)
{
    t_map *map;

    if (ac != 2)
    {
        printf("Error: please check your argument number\n");
        exit(1);
    }

    // Parse the map
    map = check_map(av[1]);

    // Load game setup (window creation, etc.)
    load_game(map);

    // Initialize image buffer for flicker-free rendering
    setup_rendering(map);
    render_scene(map);

    // Start event loop
    // mlx_loop(map->mlx);

    return (0);
}
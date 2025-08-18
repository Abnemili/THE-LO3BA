/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/18 11:52:06 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

int main (int ac, char **av)
{
    t_map   *map;

    if (ac != 2)
    {
        printf("Error : please check  your argument numebr\n");
        exit(1);
    }

    // here the parsing function 
    map = check_map(av[1]);// since were done with the parsing lets go for loeding the game 
    // and the loading the game 
    load_game(map);
}
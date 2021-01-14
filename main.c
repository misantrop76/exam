#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
    int large;
    int haut;
    char c;
}           t_zone;

typedef struct s_rec
{
    char type;
    float y;
    float x;
    float large;
    float haut;
    char c;
}           t_rec;

int    ft_strlen(char *str)
{
    int i;

    i = 0;
    while(str[i])
        i++;
    return(i);
}

int error(char *str)
{
    write(1, str, ft_strlen(str));
    return(1);
}

int get_zone(t_zone *zone, FILE *file)
{
    int ret;

    ret = fscanf(file, "%d %d %c\n", &zone->large, &zone->haut, &zone->c);
    if (ret != 3)
        return (0);
    if (zone->large < 0 || zone->large > 300 || zone->haut < 0 || zone->haut > 300)
        return (0);
    return (1);
}

char *fill_tab(t_zone *zone)
{
    char *tab;
    int i;

    if (!(tab = malloc(sizeof(char) * (zone->haut * zone->large + 1))))
        return (NULL);
    i = 0;
    while (i < zone->haut * zone->large)
    {
        tab[i] = zone->c;
        i++;
    }
    tab[i] = 0;
    return (tab);
}

void    print_tab(char *tab, t_zone *zone)
{
    int i;

    i = 0;
    while (i < zone->haut)
    {
        write(1, tab + (zone->large * i), zone->large);
        write(1, "\n", 1);
        i++;
    }
}

int is_in(float x, float y, t_rec *rec)
{

    if (x >= rec->x && x <= rec->x + rec->large && y >= rec->y && y <= rec->y + rec->haut)
    {
        if (x < rec->x + (float)1 || x > rec->x + rec->large - (float)1 || y < rec->y + (float)1 || y > rec->y + rec->haut - (float)1)
            return(2);
        return(1);
    }
    return(0);
}

int     put_rec(t_zone *zone, FILE *file, char **tab)
{
    int ret;
    int tmp;
    int i;
    int j;
    t_rec rec;


    while((ret = fscanf(file, "%c %f %f %f %f %c\n", &rec.type, &rec.x, &rec.y, &rec.large, &rec.haut, &rec.c)) == 6)
    {
        if ((rec.type != 'r' && rec.type != 'R') || rec.large < (float)0|| rec.haut < (float)0)
            return(0);
        i = 0;
        while(i < zone->haut)
        {
            j = 0;
            while(j < zone->large)
            {
                tmp = is_in((float)j, (float)i, &rec);
                if ((tmp == 2 && rec.type == 'r') || (tmp && rec.type == 'R'))
                    (*tab)[i * zone->large + j] = rec.c;
                j++;
            }
            i++;
        }
    }
    return(1);
}

int main(int ac, char **av)
{
    FILE    *file;
    t_zone zone;
    char    *tab;

    if (ac != 2)
        return(error("error argument"));
    if (!(file = fopen(av[1], "r")))
        return(error("error file"));
    if (!get_zone(&zone, file))
        return(error("error file2"));
    if (!(tab = fill_tab(&zone)))
        return (error("error malloc"));
    if (!(put_rec(&zone, file, &tab)))
        return(error("error file3"));
    print_tab(tab, &zone);
    return (1);
}
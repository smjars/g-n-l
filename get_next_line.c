/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jruiz-sa <jruiz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:18:12 by jruiz-sa          #+#    #+#             */
/*   Updated: 2024/10/01 21:10:56 by jruiz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*Esta función libera la memoria apuntada por el puntero str y establece el puntero a NULL para evitar accesos posteriores a memoria liberada.*/
char	*ft_free(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

/*Esta función limpia el almacenamiento temporal después de leer una línea.
Busca el carácter de nueva línea (\n). Si no lo encuentra, libera la memoria y retorna NULL.
Si lo encuentra, crea una nueva cadena que contiene la parte de almacenamiento después del \n y libera la memoria de la cadena original.*/
char	*clean_storage(char *storage)
{
	char	*new_storage;
	char	*ptr;
	int		len;

	ptr = ft_strchr(storage, '\n');
	if (!ptr)
	{
		new_storage = NULL;
		return (ft_free(&storage));
	}
	else
		len = (ptr - storage) + 1;
	if (!storage[len])
		return (ft_free(&storage));
	new_storage = ft_substr(storage, len, ft_strlen(storage) - len);
	ft_free(&storage);
	if (!new_storage)
		return (NULL);
	return (new_storage);
}

/*Esta función extrae una línea completa de la cadena de almacenamiento.
Encuentra el primer \n y crea una nueva cadena que contiene todos los caracteres desde el inicio hasta el \n.*/
char	*new_line(char *storage)
{
	char	*line;
	char	*ptr;
	int		len;

	ptr = ft_strchr(storage, '\n');
	len = (ptr - storage) + 1;
	line = ft_substr(storage, 0, len);
	if (!line)
		return (NULL);
	return (line);
}

/*Esta función lee del archivo descriptor (fd) y almacena los datos leídos en el almacenamiento temporal (storage).
Utiliza un búfer para leer y continúa hasta que encuentra un \n o se acaban los datos en el archivo.
Si ocurre un error de lectura, libera la memoria y retorna NULL.*/
char	*readbuf(int fd, char *storage)
{
	int		rid;
	char	*buffer;

	rid = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free(&storage));
	buffer[0] = '\0';
	while (rid > 0 && !ft_strchr(buffer, '\n'))
	{
		rid = read (fd, buffer, BUFFER_SIZE);
		if (rid > 0)
		{
			buffer[rid] = '\0';
			storage = ft_strjoin(storage, buffer);
		}
	}
	free(buffer);
	if (rid == -1)
		return (ft_free(&storage));
	return (storage);
}

/*Esta función es la principal que se llama para obtener la siguiente línea de un archivo.
Utiliza una variable estática storage para mantener datos entre llamadas.
Si storage no contiene un \n, llama a readbuf para leer más datos.
Extrae la línea usando new_line y limpia storage con clean_storage antes de retornar la línea leída.*/
char	*get_next_line(int fd)
{
	static char	*storage = {0}; /*Declara una sola variable de puntero a char llamada storage, que inicialmente apunta a NULL (o cero)*/
	char		*line;

	if (fd < 0)
		return (NULL);
	if ((storage && !ft_strchr(storage, '\n')) || !storage)
		storage = readbuf (fd, storage);
	if (!storage)
		return (NULL);
	line = new_line(storage);
	if (!line)
		return (ft_free(&storage));
	storage = clean_storage(storage);
	return (line);
}

/*#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;

    fd = open("test1.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return (1);
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return (0);
} */

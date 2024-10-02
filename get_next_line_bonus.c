/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jruiz-sa <jruiz-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:18:12 by jruiz-sa          #+#    #+#             */
/*   Updated: 2024/10/01 20:50:27 by jruiz-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/*Libera la memoria apuntada por *str y establece el puntero a NULL.
Devuelve NULL para facilitar el manejo de errores.*/
char	*ft_free(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

/*Limpia el contenido almacenado después de leer una línea completa.
Si no se encuentra un salto de línea (\n), libera la memoria de storage y devuelve NULL.
Si se encuentra un salto de línea, crea una nueva cadena que contiene el contenido después del salto de línea y libera la memoria de storage.*/
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

/*Extrae la línea completa desde el inicio de storage hasta el primer salto de línea (\n).
Devuelve la línea extraída.*/
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
/*Lee el contenido del archivo en el buffer hasta encontrar un salto de línea o llegar al final del archivo.
Concatena el contenido leído al storage.
Maneja errores de lectura liberando la memoria de storage en caso de fallo.*/
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
/*Función principal que gestiona la lectura de líneas desde un archivo.
Utiliza un arreglo estático storage para mantener el estado de múltiples descriptores de archivo.
Llama a readbuf para leer más contenido si es necesario.
Extrae y retorna la siguiente línea completa usando new_line.
Limpia storage después de extraer la línea usando clean_storage.*/
char	*get_next_line(int fd)
{
	static char	*storage[OPEN_MAX]; /*Declara un arreglo estático de punteros a char llamado storage con un tamaño de OPEN_MAX. Puede almacenar múltiples punteros, uno para cada descriptor de archivo hasta el límite de OPEN_MAX.*/
	char		*line;

	if (fd < 0)
		return (NULL);
	if ((storage[fd] && !ft_strchr(storage[fd], '\n')) || !storage[fd])
		storage[fd] = readbuf (fd, storage[fd]);
	if (!storage[fd])
		return (NULL);
	line = new_line(storage[fd]);
	if (!line)
		return (ft_free(&storage[fd]));
	storage[fd] = clean_storage(storage[fd]);
	return (line);
}

/*
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    int fd1, fd2;
    char *line;

    // Abre dos archivos para probar la funcionalidad de múltiples descriptores de archivo
    fd1 = open("test1.txt", O_RDONLY);
    fd2 = open("test2.txt", O_RDONLY);

    if (fd1 == -1 || fd2 == -1)
    {
        perror("Error opening file");
        return (1);
    }

    printf("Reading from test1.txt:\n");
    while ((line = get_next_line(fd1)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    printf("\nReading from test2.txt:\n");
    while ((line = get_next_line(fd2)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd1);
    close(fd2);
    return (0);
}*/

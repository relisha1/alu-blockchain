#include "cli.h"

/**
 * find_command - look up a command by name
 *
 * @name: name of the command typed by the user
 *
 * Return: pointer to the matching command, or NULL if none matches
 */

command_t const *find_command(char const *name)
{
	command_t const *command = get_commands();

	if (!name)
		return (NULL);

	while (command->name)
	{
		if (strcmp(command->name, name) == 0)
			return (command);
		command += 1;
	}
	return (NULL);
}

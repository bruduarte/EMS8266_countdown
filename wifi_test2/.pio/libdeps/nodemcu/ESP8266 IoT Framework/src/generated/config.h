#ifndef CONFIG_H
#define CONFIG_H

struct configData
{
	char Area[15];
	char City[15];
	char language[3];
	char projectName[32];
};

extern uint32_t configVersion;
extern const configData defaults;

#endif
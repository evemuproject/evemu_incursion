ALTER TABLE `character_`
	ADD COLUMN `freeRespecs` INT(10) UNSIGNED NULL DEFAULT '0' AFTER `online`,
	ADD COLUMN `nextRespec` BIGINT(20) UNSIGNED NULL DEFAULT '0' AFTER `freeRespecs`;
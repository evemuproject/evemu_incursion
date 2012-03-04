ALTER TABLE `corporation`
ADD `walletDivision2` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '2nd Wallet Division',
ADD `walletDivision3` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '3rd Wallet Division',
ADD `walletDivision4` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '4th Wallet Division',
ADD `walletDivision5` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '5th Wallet Division',
ADD `walletDivision6` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '6th Wallet Division',
ADD `walletDivision7` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '7th Wallet Division',
ADD `divisionBalance2` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance3` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance4` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance5` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance6` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance7` DOUBLE NOT NULL DEFAULT '0',
ADD `isRecruiting` BOOLEAN NOT NULL DEFAULT '1';

UPDATE corporation SET creatorID = ceoID;
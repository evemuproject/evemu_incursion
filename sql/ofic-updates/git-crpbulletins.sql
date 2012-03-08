CREATE TABLE `evemu-incursion`.`crpbulletins` (
`bulletinID` INT( 10 ) NULL AUTO_INCREMENT PRIMARY KEY ,
`title` VARCHAR( 100 ) NOT NULL ,
`body` TEXT NOT NULL ,
`editCharacterID` INT( 10 ) NOT NULL ,
`editDateTime` BIGINT( 20 ) NOT NULL ,
`corporationID` INT( 10 ) NOT NULL
) ENGINE = InnoDB;
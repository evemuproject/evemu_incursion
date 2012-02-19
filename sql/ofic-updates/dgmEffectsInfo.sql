
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

-- --------------------------------------------------------

--
-- Table structure for table `dgmEffectsInfo`
--

CREATE TABLE IF NOT EXISTS `dgmEffectsInfo` (
  `effectID` int(11) NOT NULL,
  `targetAttributeID` int(11) NOT NULL,
  `sourceAttributeID` int(11) NOT NULL,
  `calculationTypeID` int(11) NOT NULL,
  `reverseCalculationTypeID` int(11) NOT NULL,
  `effectApplied` int(11) NOT NULL,
  `effectAppliedTo` int(11) NOT NULL,
  PRIMARY KEY (`effectID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `dgmEffectsInfo`
--

INSERT INTO `dgmEffectsInfo` (`effectID`, `targetAttributeID`, `sourceAttributeID`, `calculationTypeID`, `reverseCalculationTypeID`, `effectApplied`, `effectAppliedTo`) VALUES
(1959, 4, 796, 1, 2, 0, 1),
(2837, 265, 1159, 1, 2, 0, 1);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

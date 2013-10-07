-- 
-- Struktura tabulky `labrea`
-- 

-- CREATE TABLE `labrea` (
--  `time` int(11) NOT NULL,
--  `bw` int(11) NOT NULL,
--  KEY `bytime` (`time`)
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8 MAX_ROWS=4294967295 AVG_ROW_LENGTH=512;

-- --------------------------------------------------------

-- 
-- Struktura tabulky `tarpit`
-- 

CREATE TABLE `tarpit` (
  `time` int(11) NOT NULL,
  `ipsrc` char(46) NOT NULL,
  `psrc` int(11) NOT NULL,
  `ipdst` char(46) NOT NULL,
  `pdst` int(11) NOT NULL,
  KEY `bytime` (`time`),
  KEY `byipsrc` (`ipsrc`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 MAX_ROWS=4294967295 AVG_ROW_LENGTH=512;


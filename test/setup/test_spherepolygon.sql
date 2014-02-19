.load ../build/libsqlitesphere.so
CREATE TABLE IF NOT EXISTS spherepolygon (polygon BLOB);
INSERT INTO spherepolygon (polygon) VALUES (spoly(" {(0d,30d),(45d,30d),(90d,30d),(135d,30d),(180d,30d),(225d,30d),(270d,30d),(315d,30d)} " ));
INSERT INTO spherepolygon (polygon) VALUES (spoly(" {(0d,-5d),(1d,-5d),(2d,-5d),(3d,-5d),(4d,-5d),(5d,-5d),(6d,-5d),(7d,-5d),(8d,-5d),(9d,-5d),(10d,-5d),(11d,-5d),(12d,-5d),(13d,-5d),(14d,-5d),(15d,-5d),(16d,-5d),(17d,-5d),(18d,-5d),(19d,-5d),(20d,-5d),(21d,-5d),(22d,-5d),(23d,-5d),(24d,-5d),(25d,-5d),(26d,-5d),(27d,-5d),(28d,-5d),(29d,-5d),(30d,-5d),(31d,-5d),(32d,-5d),(33d,-5d),(34d,-5d),(35d,-5d),(36d,-5d),(37d,-5d),(38d,-5d),(39d,-5d),(40d,-5d),(41d,-5d),(42d,-5d),(43d,-5d),(44d,-5d),(45d,-5d),(46d,-5d),(47d,-5d),(48d,-5d),(49d,-5d),(50d,-5d),(51d,-5d),(52d,-5d),(53d,-5d),(54d,-5d),(55d,-5d),(56d,-5d),(57d,-5d),(58d,-5d),(59d,-5d),(60d,-5d),(61d,-5d),(62d,-5d),(63d,-5d),(64d,-5d),(65d,-5d),(66d,-5d),(67d,-5d),(68d,-5d),(69d,-5d),(70d,-5d),(71d,-5d),(72d,-5d),(73d,-5d),(74d,-5d),(75d,-5d),(76d,-5d),(77d,-5d),(78d,-5d),(79d,-5d),(80d,-5d),(81d,-5d),(82d,-5d),(83d,-5d),(84d,-5d),(85d,-5d),(86d,-5d),(87d,-5d),(88d,-5d),(89d,-5d),(90d,-5d),(91d,-5d),(92d,-5d),(93d,-5d),(94d,-5d),(95d,-5d),(96d,-5d),(97d,-5d),(98d,-5d),(99d,-5d),(100d,-5d),(101d,-5d),(102d,-5d),(103d,-5d),(104d,-5d),(105d,-5d),(106d,-5d),(107d,-5d),(108d,-5d),(109d,-5d),(110d,-5d),(111d,-5d),(112d,-5d),(113d,-5d),(114d,-5d),(115d,-5d),(116d,-5d),(117d,-5d),(118d,-5d),(119d,-5d),(120d,-5d),(121d,-5d),(122d,-5d),(123d,-5d),(124d,-5d),(125d,-5d),(126d,-5d),(127d,-5d),(128d,-5d),(129d,-5d),(130d,-5d),(131d,-5d),(132d,-5d),(133d,-5d),(134d,-5d),(135d,-5d),(136d,-5d),(137d,-5d),(138d,-5d),(139d,-5d),(140d,-5d),(141d,-5d),(142d,-5d),(143d,-5d),(144d,-5d),(145d,-5d),(146d,-5d),(147d,-5d),(148d,-5d),(149d,-5d),(150d,-5d),(151d,-5d),(152d,-5d),(153d,-5d),(154d,-5d),(155d,-5d),(156d,-5d),(157d,-5d),(158d,-5d),(159d,-5d),(160d,-5d),(161d,-5d),(162d,-5d),(163d,-5d),(164d,-5d),(165d,-5d),(166d,-5d),(167d,-5d),(168d,-5d),(169d,-5d),(170d,-5d),(171d,-5d),(172d,-5d),(173d,-5d),(174d,-5d),(175d,-5d),(176d,-5d),(177d,-5d),(178d,-5d),(179d,-5d),(180d,-5d),(181d,-5d),(182d,-5d),(183d,-5d),(184d,-5d),(185d,-5d),(186d,-5d),(187d,-5d),(188d,-5d),(189d,-5d),(190d,-5d),(191d,-5d),(192d,-5d),(193d,-5d),(194d,-5d),(195d,-5d),(196d,-5d),(197d,-5d),(198d,-5d),(199d,-5d),(200d,-5d),(201d,-5d),(202d,-5d),(203d,-5d),(204d,-5d),(205d,-5d),(206d,-5d),(207d,-5d),(208d,-5d),(209d,-5d),(210d,-5d),(211d,-5d),(212d,-5d),(213d,-5d),(214d,-5d),(215d,-5d),(216d,-5d),(217d,-5d),(218d,-5d),(219d,-5d),(220d,-5d),(221d,-5d),(222d,-5d),(223d,-5d),(224d,-5d),(225d,-5d),(226d,-5d),(227d,-5d),(228d,-5d),(229d,-5d),(230d,-5d),(231d,-5d),(232d,-5d),(233d,-5d),(234d,-5d),(235d,-5d),(236d,-5d),(237d,-5d),(238d,-5d),(239d,-5d),(240d,-5d),(241d,-5d),(242d,-5d),(243d,-5d),(244d,-5d),(245d,-5d),(246d,-5d),(247d,-5d),(248d,-5d),(249d,-5d),(250d,-5d),(251d,-5d),(252d,-5d),(253d,-5d),(254d,-5d),(255d,-5d),(256d,-5d),(257d,-5d),(258d,-5d),(259d,-5d),(260d,-5d),(261d,-5d),(262d,-5d),(263d,-5d),(264d,-5d),(265d,-5d),(266d,-5d),(267d,-5d),(268d,-5d),(269d,-5d),(270d,-5d),(271d,-5d),(272d,-5d),(273d,-5d),(274d,-5d),(275d,-5d),(276d,-5d),(277d,-5d),(278d,-5d),(279d,-5d),(280d,-5d),(281d,-5d),(282d,-5d),(283d,-5d),(284d,-5d),(285d,-5d),(286d,-5d),(287d,-5d),(288d,-5d),(289d,-5d),(290d,-5d),(291d,-5d),(292d,-5d),(293d,-5d),(294d,-5d),(295d,-5d),(296d,-5d),(297d,-5d),(298d,-5d),(299d,-5d),(300d,-5d),(301d,-5d),(302d,-5d),(303d,-5d),(304d,-5d),(305d,-5d),(306d,-5d),(307d,-5d),(308d,-5d),(309d,-5d),(310d,-5d),(311d,-5d),(312d,-5d),(313d,-5d),(314d,-5d),(315d,-5d),(316d,-5d),(317d,-5d),(318d,-5d),(319d,-5d),(320d,-5d),(321d,-5d),(322d,-5d),(323d,-5d),(324d,-5d),(325d,-5d),(326d,-5d),(327d,-5d),(328d,-5d),(329d,-5d),(330d,-5d),(331d,-5d),(332d,-5d),(333d,-5d),(334d,-5d),(335d,-5d),(336d,-5d),(337d,-5d),(338d,-5d),(339d,-5d),(340d,-5d),(341d,-5d),(342d,-5d),(343d,-5d),(344d,-5d),(345d,-5d),(346d,-5d),(347d,-5d),(348d,-5d),(349d,-5d),(350d,-5d),(351d,-5d),(352d,-5d),(353d,-5d),(354d,-5d),(355d,-5d),(356d,-5d),(357d,-5d),(358d,-5d),(359d,-5d)} " ));

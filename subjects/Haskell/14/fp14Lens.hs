{-# LANGUAGE TemplateHaskell #-}
module Fp14lens where
import Control.Lens

{-
Линза - инструмент для манипулирования подструктурой типа a 
некоторой структуры данных типа s.
Технически линза - это АТД составленный из пары геттер-сеттер
lens :: (s -> a) -> (s -> a -> s) -> Lens' s a
-}

data Arc = Arc { 
  _degree, _minute, _second :: Int 
  } deriving Show
  
data Location = Location { 
  _latitude, _longitude :: Arc 
  } deriving Show
-- символ подчеркивания в полях записи является конвенцией, принятой в Control.Lens для генерации линз с помощью TH.

-- TH создает нужные функции ...
$(makeLenses ''Location)
-- ... а именно
-- latitude  :: Lens' Location Arc
-- longitude :: Lens' Location Arc
-- рассахаренные типы такие
-- latitude :: Functor f => (Arc -> f Arc) -> Location -> f Location

$(makeLenses ''Arc)

-- degree :: Lens' Arc Int
-- minute :: Lens' Arc Int
-- second :: Lens' Arc Int

{-
GHCi> auLocation = Location (Arc 60 0 9) (Arc 30 22 26)
GHCi> auLocation ^. latitude . degree
60
GHCi> auLocation & longitude . second .~ 27
Location {_latitude = Arc {_degree = 60, _minute = 0, _second = 9}, _longitude =
 Arc {_degree = 30, _minute = 22, _second = 27}}
-}


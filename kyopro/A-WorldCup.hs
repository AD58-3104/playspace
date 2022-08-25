import Data.Char
import Data.List

solve:: Integer -> Integer 
solve n
    | n `mod` 4 == 2 = n
    | otherwise = solve (n + 1)

main = do 
    y <- readLn 
    print $ solve y 
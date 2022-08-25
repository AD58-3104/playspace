import Data.Char
import Data.List

main = do 
    tmp <- fmap (read :: String -> Int) . words <$> getLine
    let list = sort $ tmp
    let a = length $ filter  (\num -> (head list) == num)  list
    let b = length $ filter  (\num -> (last list) == num)  list
    if (a * b == 6) then putStrLn "Yes" else putStrLn "No"
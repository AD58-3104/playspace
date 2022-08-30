import Control.Monad
import Data.List

solve::String -> String 
solve str 
    | null str  = "-1"
    | length (filter (\x -> x == (head str)) str) == 1 = [head str]
    | otherwise = solve $ filter (\x -> x /= (head str)) str

main = do
    str <- getLine 
    let ans = solve str
    putStrLn ans
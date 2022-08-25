import Data.Char

main = do 
    [l,r] <- map read . words <$> getLine
    let atc = "atcoder"
    putStrLn $ take (r - l + 1) $ (drop (l-1) atc)
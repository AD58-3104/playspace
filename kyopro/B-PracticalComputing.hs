import Data.Tree (flatten)
generatebinom :: [Int] -> [Int]
generatebinom (h1:last) 
    | null last = [1]
    | otherwise = [h1+ (head last)] ++ generatebinom last
generatebinom last = [1]  


solve :: Int -> [[Int]]
solve 0 = error "input 0 is not allowed"
solve 1 = [[1]]
solve 2 = [[1,1]] ++ solve 1 
solve n = [ [1] ++ generatebinom (head (solve (n -1))) ] ++ solve (n - 1)

reverseLL ::[[Int]] -> [[Int]]
reverseLL (left:lastingRight) 
    | null lastingRight = [left]
    | otherwise = (reverseLL lastingRight) ++ [left]

main = do
    n <- readLn
    let ans = reverseLL (solve n)
    let ansStr = fmap (unwords . fmap show) ans
    putStrLn $ unlines ansStr
main = do 
    [a,b,n] <- map read . words <$> getLine
    if ( a < ( b `div` 3 ) ) then putStrLn $ show ( a * n ) else putStrLn $ show ( b * ( n `div` 3 ) + a * (n `mod` 3))
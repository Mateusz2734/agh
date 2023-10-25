roots :: (Double, Double, Double) -> (Double, Double)
roots (a, b, c) = ((-b - d) / e, (-b + d) / e)
  where
    d = sqrt (b * b - 4 * a * c)
    e = 2 * a

unitVec2D :: (Double, Double) -> (Double, Double)
unitVec2D (x, y) = (x / l, y / l) where l = sqrt (x ** 2 + y ** 2)

unitVec3D :: (Double, Double, Double) -> (Double, Double, Double)
unitVec3D (x, y, z) = (x / l, y / l, z / l)
  where
    l = sqrt (x ** 2 + y ** 2 + z ** 2)

heron :: (Double, Double, Double) -> Double
heron (a, b, c) = sqrt (p * (p - a) * (p - b) * (p - c))
  where
    p = (a + b + c) / 2
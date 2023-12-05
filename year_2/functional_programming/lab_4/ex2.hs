data CartInt2DVec = MkCartInt2DVec Int Int

xCoord :: CartInt2DVec -> Int
xCoord (MkCartInt2DVec x _) = x

yCoord :: CartInt2DVec -> Int
yCoord (MkCartInt2DVec _ y) = y

data Cart2DVec' a = MkCart2DVec' a a

xCoord' :: Cart2DVec' a -> a
xCoord' (MkCart2DVec' x _) = x

yCoord' :: Cart2DVec' a -> a
yCoord' (MkCart2DVec' _ y) = y

data Cart2DVec'' a = MkCart2DVec'' {x :: a, y :: a}

-- sum type example (two constructors)
data List a = EmptyL | Cons a (List a) deriving (Show)

head' :: List a -> a
head' EmptyL = error "head': the empty list has no head!"
head' (Cons x xs) = x

-- enum type example (special case of sum type)
data ThreeColors
  = Blue
  | White
  | Red

type ActorName = String

leadingActor :: ThreeColors -> ActorName
leadingActor Blue = "Juliette Binoche"
leadingActor White = "Zbigniew Zamachowski"
leadingActor Red = "Irene Jacob"

data Cart3DVec a = Cart3DVec a a a

xCoord3D :: Cart3DVec a -> a
xCoord3D (Cart3DVec x _ _) = x

yCoord3D :: Cart3DVec a -> a
yCoord3D (Cart3DVec _ y _) = y

zCoord3D :: Cart3DVec a -> a
zCoord3D (Cart3DVec _ _ z) = z

data Cart3DVecRs a = Cart3DVecRs {xCoord3DRs :: a, yCoord3DRs :: a, zCoord3DRs :: a}

data Shape
  = Circle Float
  | Rectangle Float Float

area :: Shape -> Float
area (Circle r) = pi * r * r
area (Rectangle x y) = x * y

data TrafficLights = RED | YELLOW | GREEN

actionFor :: TrafficLights -> String
actionFor RED = "STOP! DO NOT DRIVE!"
actionFor YELLOW = "SLOW DOWN!"
actionFor GREEN = "GO AHEAD"
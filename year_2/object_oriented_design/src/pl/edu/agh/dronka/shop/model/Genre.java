package pl.edu.agh.dronka.shop.model;

public enum Genre {
    Pop("Pop"), Rock("Rock"), Metal("Metal"), Undefined("Nieznany");
    private final String displayName;

    public String getDisplayName() {
        return displayName;
    }
    private Genre(String displayName) {
        this.displayName = displayName;
    }

    public static Genre fromString(String name) {
        return switch (name) {
            case "Pop" -> Pop;
            case "Rock" -> Rock;
            case "Metal" -> Metal;
            default -> Undefined;
        };
    }
}

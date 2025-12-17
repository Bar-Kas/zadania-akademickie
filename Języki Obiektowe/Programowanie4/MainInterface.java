public class MainInterface {
    public static void main(String[] args) {
        HealthPotion potion = new HealthPotion();
        TeleportScroll scroll = new TeleportScroll();
        Trap trap = new Trap();
        Item[] inventory = {potion, scroll, trap};

        System.out.println("--- Używanie przedmiotów z plecaka ---");
        for (Item i : inventory) {
            i.use();
        }
    }
}

abstract class Character {
    protected String name;
    protected float health;
    protected float armor= 0.0F;
    protected String charClass;

    public Character(String name, float health) {
        this.name = name;
        this.health = health;
    }

    public void changeArmour(float armor) {
        this.armor = armor;
    }

    public void normalAttack(){
        System.out.println(this.name +" executes a normal attack!");
    }
    public abstract void specialAttack();
}

class Warrior extends Character {

    public Warrior(String name, float health) {
        super(name, health);
        charClass="Warrior";
    }

    public void specialAttack() {
        System.out.println(this.name + " (Warrior) executes a powerful Shield Bash!");
    }
}

class Mage extends Character{
    public Mage(String name, float health) {
        super(name, health);
        charClass="Mage";
    }

    public void specialAttack() {
        System.out.println(this.name + " (Mage) executes a powerful Spell!");
    }
}

class Archer extends Character{
    public Archer(String name, float health) {
        super(name, health);
        charClass="Archer";
    }

    public void specialAttack() {
        System.out.println(this.name + " (Archer) executes a arrow Barage!");
    }
}

interface Item {
    void use();
}
class HealthPotion implements Item {
    @Override
    public void use() {
        System.out.println("You've used a potion.");
    }
}

class TeleportScroll implements Item {
    @Override
    public void use() {
        System.out.println("You've used a teleportaion scroll.");
    }
}

class Trap implements Item {
    @Override
    public void use() {
        System.out.println("You've used a trap.");
    }
}

public class Main {
    public static void main(String[] args) {
        Warrior warrior = new Warrior("Geralt", 100);
        Mage mage = new Mage("Yennefer", 60);
        Archer archer = new Archer("Milva", 75);
        Character[] party = {warrior, mage, archer};

        System.out.println("--- Normal Attacks ---");

        for (Character c : party) {
            c.specialAttack();
        }

        System.out.println("--- Special Attacks ---");

        for (Character c : party) {
            c.normalAttack();
        }

        HealthPotion potion = new HealthPotion();
        TeleportScroll scroll = new TeleportScroll();
        Trap trap = new Trap();
        Item[] inventory = {potion, scroll, trap};

        System.out.println("--- Use all items in backpack ---");
        for (Item i : inventory) {
            i.use();
        }
        
    }
}
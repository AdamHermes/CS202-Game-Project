#pragma once
enum class WeaponType {
	Sword,
	Spear,
	Bow,
    None
};
class Weapon {
private:
	float damage;
    WeaponType weaponType;
public:
    Weapon(WeaponType type) {
        weaponType = type;
        switch (type) {
        case WeaponType::Sword:
            damage = 13;
            break;
        case WeaponType::Spear:
            damage = 15;
            break;
        case WeaponType::Bow:
            damage = 10;
            break;
        case WeaponType::None:
            damage = 0;
            break;
        }
    }
    float getDamage() const { return damage; }
    WeaponType getType() {
        return weaponType;
    }
};
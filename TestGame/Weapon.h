#pragma once
enum class WeaponType {
	Sword,
	Spear,
	Bow
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
            damage = 10;
            break;
        case WeaponType::Spear:
            damage = 10;
            break;
        case WeaponType::Bow:
            damage = 15;
            break;
        }
    }
    float getDamage() const { return damage; }

};
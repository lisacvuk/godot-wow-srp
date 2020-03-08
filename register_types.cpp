
#include "register_types.h"

#include "core/class_db.h"
#include "wow_srp.h"

void register_wow_srp_types() {
    ClassDB::register_class<Wow_SRP>();
}

void unregister_wow_srp_types() {
}
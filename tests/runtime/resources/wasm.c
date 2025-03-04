#include <assert.h>
#include <limits.h>
#include <math.h>
#include <resources.h>

struct exports_x_t {
    int32_t a;
};

struct exports_z_t {
    int32_t a;
};

resources_own_x_t exports_constructor_x(int32_t a) {
    exports_x_t* x_instance = (exports_x_t*)malloc(sizeof(exports_x_t));
    x_instance->a = a;
    resources_own_x_t x_own = exports_x_new(x_instance);
    return x_own;
}

int32_t exports_method_x_get_a(exports_x_t* self) {
    return self->a;
}

void exports_method_x_set_a(exports_x_t* self, int32_t a) {
    self->a = a;
}

resources_own_x_t exports_static_x_add(resources_own_x_t x, int32_t a) {
    exports_x_t* x_instance = exports_x_rep(x);
    x_instance->a += a;
    return x;
}

resources_own_z_t exports_constructor_z(int32_t a) {
    exports_z_t* z_instance = (exports_z_t*)malloc(sizeof(exports_z_t));
    z_instance->a = a;
    resources_own_z_t z_own = exports_z_new(z_instance);
    return z_own;
}

int32_t exports_method_z_get_a(exports_z_t* self) {
    return self->a;
}

resources_own_z_t exports_add(exports_z_t* a, exports_z_t* b) {
    int32_t c = a->a + b->a;
    return exports_constructor_z(c);
}

void exports_x_destructor(exports_x_t* x) {
    free(x);
}

void exports_z_destructor(exports_z_t* z) {
    free(z);
}

bool exports_test_imports(resources_string_t *err) {
    resources_own_y_t y = imports_constructor_y(10);
    resources_borrow_y_t borrowed_y = imports_borrow_y(y);
    assert(imports_method_y_get_a(borrowed_y) == 10);
    imports_method_y_set_a(borrowed_y, 20);
    assert(imports_method_y_get_a(borrowed_y) == 20);
    
    resources_own_y_t y2 = imports_static_y_add(y, 10);
    resources_borrow_y_t borrowed_y2 = imports_borrow_y(y2);
    assert(imports_method_y_get_a(borrowed_y2) == 30);

    imports_y_drop_own(y);

    // multiple instances 
    resources_own_y_t y1 = imports_constructor_y(1);
    resources_own_y_t y2_m = imports_constructor_y(2);
    resources_borrow_y_t borrowed_y1 = imports_borrow_y(y1);
    resources_borrow_y_t borrowed_y2_m = imports_borrow_y(y2_m);
    assert(imports_method_y_get_a(borrowed_y1) == 1);
    assert(imports_method_y_get_a(borrowed_y2_m) == 2);
    imports_method_y_set_a(borrowed_y1, 10);
    imports_method_y_set_a(borrowed_y2_m, 20);
    assert(imports_method_y_get_a(borrowed_y1) == 10);
    assert(imports_method_y_get_a(borrowed_y2_m) == 20);
    resources_own_y_t y3 = imports_static_y_add(y1, 20);
    resources_own_y_t y4 = imports_static_y_add(y2_m, 30);
    resources_borrow_y_t borrowed_y3 = imports_borrow_y(y3);
    resources_borrow_y_t borrowed_y4 = imports_borrow_y(y4);
    assert(imports_method_y_get_a(borrowed_y3) == 30);
    assert(imports_method_y_get_a(borrowed_y4) == 50);
    imports_y_drop_own(y1);
    imports_y_drop_own(y2_m);

    return true;
}
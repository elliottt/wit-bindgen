use std::cell::RefCell;

wit_bindgen::generate!({
    path: "../../tests/runtime/resource_import_and_export",
    exports: {
        world: Test,
        "test:resource-import-and-export/test/thing": MyThing,
    }
});

use exports::test::resource_import_and_export::test::GuestThing;
use exports::test::resource_import_and_export::test::OwnThing;
use test::resource_import_and_export::test::Thing;

pub struct Test {}

pub struct MyThing {
    thing: RefCell<Option<Thing>>,
}

impl GuestThing for MyThing {
    fn new(v: u32) -> Self {
        Self {
            thing: RefCell::new(Some(Thing::new(v + 1))),
        }
    }

    fn foo(&self) -> u32 {
        let thing = self.thing.borrow();
        let thing = thing.as_ref().unwrap();
        thing.foo() + 2
    }

    fn bar(&self, v: u32) {
        let mut thing = self.thing.borrow_mut();
        let thing = thing.as_mut().unwrap();
        thing.bar(v + 3);
    }

    fn baz(a: OwnThing, b: OwnThing) -> OwnThing {
        let mut a = a.thing.borrow_mut();
        let mut b = b.thing.borrow_mut();
        let result =
            Thing::baz(Option::take(&mut a).unwrap(), Option::take(&mut b).unwrap()).foo() + 4;
        OwnThing::new(MyThing::new(result))
    }
}

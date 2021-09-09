mergeInto(LibraryManager.library, {
  displayInner: function (html) {
    var element = document.getElementById("output_dcw");
    if (!element) {
      var element = document.createElement("div");
      element.id = "output_dcw";
      document.body.appendChild(element);
    }
    element.innerHTML += UTF8ToString(html);
  },
  removeStaticInner: function (html) {
    var element = document.getElementById("output_dcw");
    if (!element) {
      var element = document.createElement("div");
      element.id = "output_dcw";
      document.body.appendChild(element);
    }
    element.innerHTML = element.innerHTML.replace(UTF8ToString(html), "");
  },
  removeWithClassInner: function (className) {
    let element = document.querySelectorAll(`.${UTF8ToString(className)}`);
    if (element) {
      element.forEach(function (e) {
        e.remove();
      });
    }
  },
  removeChildrenOfIdInner: function (id) {
    let element = document.querySelector(`#${UTF8ToString(id)}`);
    while (element.lastChild) {
      element.removeChild(element.lastChild);
    }
  },
  insertAtIdInner: function (html, id) {
    let element = document.querySelector(`#${UTF8ToString(id)}`);
    if (!element) {
      element = document.createElement("div");
      element.id = UTF8ToString(id);
      document.body.appendChild(element);
    }
    element.innerHTML += UTF8ToString(html);
  }
});

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
  removeInner: function (html) {
    var element = document.getElementById("output_dcw");
    if (!element) {
      var element = document.createElement("div");
      element.id = "output_dcw";
      document.body.appendChild(element);
    }
    element.innerHTML = element.innerHTML.replace(
      UTF8ToString(html),
      ""
    );
  },
});

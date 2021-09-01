mergeInto(LibraryManager.library, {
  displayInner: function (html) {
    var element = document.createElement("div");

    if (element) element.innerHTML = ""; // clear browser cache
    if (element) {
      element.innerHTML += UTF8ToString(html);
    }
    document.body.appendChild(element);
  }
});

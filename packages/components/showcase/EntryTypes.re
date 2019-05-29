type compEntry = {
  mutable id: string,
  title: string,
  render: unit => React.element,
}
and folderEntry = {
  mutable id: string,
  title: string,
  children: list(navEntry),
}
and navEntry =
  | CompEntry(compEntry)
  | FolderEntry(folderEntry);

let entry = (~title, ~render): navEntry => {
  CompEntry({id: "", title, render});
};

let folder = (~title, ~children): navEntry => {
  FolderEntry({id: "", title, children});
};

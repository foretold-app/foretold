module Node = {
  type node =
    | Root
    | Text
    | Break
    | Emphasis
    | Paragraph
    | Strong
    | ThematicBreak
    | Blockquote
    | Delete
    | Link
    | Image
    | LinkReference
    | ImageReference
    | Table
    | TableHead
    | TableBody
    | TableRow
    | TableCell
    | List
    | ListItem
    | Definition
    | Heading
    | InlineCode
    | Code
    | Html;
  let to_string =
    fun
    | Root => "root"
    | Text => "text"
    | Break => "break"
    | Paragraph => "paragraph"
    | Emphasis => "emphasis"
    | Strong => "strong"
    | ThematicBreak => "thematicBreak"
    | Blockquote => "blockquote"
    | Delete => "delete"
    | Link => "link"
    | Image => "image"
    | LinkReference => "linkReference"
    | ImageReference => "imageReference"
    | Table => "table"
    | TableHead => "tableHead"
    | TableBody => "tableBody"
    | TableRow => "TableRow"
    | TableCell => "TableCell"
    | List => "list"
    | ListItem => "listItem"
    | Definition => "definition"
    | Heading => "heading"
    | InlineCode => "inlineCode"
    | Code => "code"
    | Html => "html";
};
let option_map = (fn, option) =>
  switch (option) {
  | Some(value) => Some(fn(value))
  | None => None
  };
let arr_map = (fn: Node.node => string, arr: option(array(Node.node))) =>
  switch (arr) {
  | Some(value) => Some(Array.map(fn, value))
  | None => None
  };

[@bs.module] external reactComponent: React.component('a) = "react-markdown";

[@bs.obj]
external makePropsText:
  (
    ~source: string,
    ~className: option(string)=?,
    ~escapeHtml: option(bool)=?,
    ~skipHtml: option(bool)=?,
    ~sourcePos: option(bool)=?,
    ~allowedTypes: option(array(Node.node))=?,
    ~disallowedTypes: option(array(Node.node))=?,
    ~unwrapDisallowed: option(bool)=?,
    ~allowNode: option((Node.node, int32, Node.node) => bool)=?,
    ~transformLinkUri: option(string => string)=?,
    ~transformImageUri: option(string => string)=?,
    ~renderers: option(Js.t({..}))=?
  ) =>
  _ =
  "";

[@react.component]
let make =
    (
      ~source: string,
      ~className: option(string)=?,
      ~escapeHtml: option(bool)=?,
      ~skipHtml: option(bool)=?,
      ~sourcePos: option(bool)=?,
      ~allowedTypes: option(array(Node.node))=?,
      ~disallowedTypes: option(array(Node.node))=?,
      ~unwrapDisallowed: option(bool)=?,
      ~allowNode: option((Node.node, int32, Node.node) => bool)=?,
      ~transformLinkUri: option(string => string)=?,
      ~transformImageUri: option(string => string)=?,
      ~renderers: option(Js.t({..}))=?,
    ) =>
  React.createElement(
    reactComponent,
    makePropsText(
      ~source,
      ~className,
      ~escapeHtml,
      ~skipHtml,
      ~sourcePos,
      ~allowedTypes,
      ~disallowedTypes,
      ~unwrapDisallowed,
      ~allowNode,
      ~transformLinkUri,
      ~transformImageUri,
      ~renderers,
    ),
  );
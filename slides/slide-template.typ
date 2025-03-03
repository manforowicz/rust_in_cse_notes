#import "@preview/polylux:0.4.0": *

#let theme_color = rgb(10, 140, 50)

#let project(body) = {
  // Basic configuration
  set page(paper: "presentation-16-9", margin: 1.5cm)
  set text(font: "Noto Sans", lang: "en", size: 20pt)
  
  
  // show raw.where(block: true): set text(size: 1.0em)
  // General styles
  set text(hyphenate: false)
  set math.mat(delim: "[")
  set math.vec(delim: "[")
  show math.cases: math.display
  set enum(numbering: "1.a.i.1.a.i.")
  show link: underline
  show math.ast.op: math.dot
  // set box(baseline: 100%)
  // set stack(dir: ltr, spacing: 1fr)

  // {
    
  //   text(weight: "bold", size: 30pt, fill: theme_color, title)
  // }

  show heading: it => {
    set text(fill: theme_color)
    
    if it.level == 1 {
      set text(size: 1.2em)
      set align(center + horizon)
      block(below: 0.8em, it.body)
    
    } else if it.level == 2 {
      set text(size: 0.9em)
      block(below: 0.8em, it.body)
    
    } else {
      set text(size: 0.8em)
      block(below: 0.8em, it.body)
    }
  }
  body
}

